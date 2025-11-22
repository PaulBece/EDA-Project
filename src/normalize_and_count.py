import os
import re
import csv
import unicodedata
from collections import Counter
import sys
import time
import spacy
import spacy.cli

LANG = 'en'   
REMOVE_STOPWORDS = True
MIN_TOKEN_LEN = 2

MODEL_BY_LANG = {
    'en': 'en_core_web_sm'
}

PROJECT_ROOT = os.path.dirname(os.path.dirname(__file__))
INPUT_FOLDER = os.path.join(PROJECT_ROOT, "input", "documents")
OUTPUT_FOLDER = os.path.join(PROJECT_ROOT, "pre-processedData", "documents")

os.makedirs(INPUT_FOLDER, exist_ok=True)
os.makedirs(OUTPUT_FOLDER, exist_ok=True)

def normalize_text(text):
    text = unicodedata.normalize("NFKD", text)
    text = "".join(c for c in text if not unicodedata.combining(c))
    text = text.lower()
    text = re.sub(r'https?://\S+|www\.\S+', ' ', text)
    text = re.sub(r"[^a-z0-9'áéíóúüñ]+", " ", text)
    text = re.sub(r'\s+', ' ', text).strip()
    return text

def lemmatize_and_count(text, nlp, remove_stopwords=True, min_len=2):
    doc = nlp(text)
    counts = Counter()
    for token in doc:
        if token.is_space or token.is_punct: continue
        lemma = token.lemma_.lower().strip()
        if remove_stopwords and token.is_stop: continue
        if len(lemma) < min_len: continue
        counts[lemma] += 1
    return counts

def save_csv(counter, filepath):
    rows = sorted(counter.items(), key=lambda x: x[1], reverse=True)
    with open(filepath, "w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        writer.writerow(["word", "frequency"])
        writer.writerows(rows)

def ensure_model(model_name):
    try:
        spacy.load(model_name)
    except OSError:
        spacy.cli.download(model_name)

def main():
    delay_ms = 1000
    if len(sys.argv) > 1:
        try:
            delay_ms = int(sys.argv[1])
        except ValueError:
            print("Invalid delay argument. Using default 1000ms.")
    
    print("Starting Normalization and Lemmatization")
    model_name = MODEL_BY_LANG.get(LANG, 'en_core_web_sm')
    ensure_model(model_name)
    nlp = spacy.load(model_name, disable=["parser", "ner"])


    files = sorted([f for f in os.listdir(INPUT_FOLDER) if f.endswith(".txt")])

    for i, filename in enumerate(files):
        input_path = os.path.join(INPUT_FOLDER, filename)
        
        output_filename = f"{i}.csv"
        final_path = os.path.join(OUTPUT_FOLDER, output_filename)
        temp_path = os.path.join(OUTPUT_FOLDER, f"{i}.tmp")

        if (i%50==0):
            print(f"Processing {filename} -> {output_filename}")

        with open(input_path, "r", encoding="utf-8", errors="ignore") as f:
            text = f.read()

        text = normalize_text(text)
        counts = lemmatize_and_count(text, nlp, REMOVE_STOPWORDS, MIN_TOKEN_LEN)
        save_csv(counts, temp_path)

        os.rename(temp_path, final_path)

        time.sleep(0.05)
        time.sleep(delay_ms / 1000.0)

    print("Finished processing all files.")

if __name__ == "__main__":
    main()