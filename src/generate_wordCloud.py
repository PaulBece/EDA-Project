import pandas as pd
from wordcloud import WordCloud
import matplotlib.pyplot as plt
import os
import time

PROJECT_ROOT = os.path.dirname(os.path.dirname(__file__))
CSV_INPUT_FOLDER = os.path.join(PROJECT_ROOT, "output", "documents")

def generate_and_update_wordcloud(csv_filepath, filename):
    try:
        df = pd.read_csv(csv_filepath)
        if 'word' not in df.columns or 'frequency' not in df.columns:
            return

        data = dict(zip(df['word'], df['frequency']))
        if not data: return

        wc = WordCloud(width=1280, height=720,
                       background_color='white',
                       max_words=100,
                       colormap='plasma').generate_from_frequencies(data)

        plt.clf() 
        plt.imshow(wc, interpolation='bilinear')
        plt.axis('off')
        plt.title(f"Live Word Cloud: {filename}")
        plt.draw()
        plt.pause(0.1)

    except Exception as e:
        print(f"Error processing {filename}: {e}")

def main():
    print("Waiting for Data")
    
    if not os.path.exists(CSV_INPUT_FOLDER):
        print(f"Waiting for folder creation: {CSV_INPUT_FOLDER}")
        while not os.path.exists(CSV_INPUT_FOLDER):
            time.sleep(1)

    plt.ion()
    plt.figure(figsize=(10, 6))

    file_counter = 0

    while True:
        target_file = f"{file_counter}.csv"
        full_path = os.path.join(CSV_INPUT_FOLDER, target_file)

        if os.path.exists(full_path):
            time.sleep(0.01) 
            
            generate_and_update_wordcloud(full_path, target_file)
            file_counter += 1
        else:
            plt.pause(0.001) 

if __name__ == "__main__":
    main()