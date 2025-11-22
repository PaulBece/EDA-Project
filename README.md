# EDA-Project

 # Prerequisites
   Python 3.10
   C++17

 # Create the venv
   python -m venv venv

 # start venv for windows
   .\venv\Scripts\activate

 # install requirements.txt
   pip install -r requirements.txt

 # compilation
   g++ src/main.cpp -I include -o main.exe

 # execution pipeline
   Step 0: Clean Environment (Optional)
      python src/reset.py

   # The Visualizer, HashTable and Normalizer will need a separete terminal

   Step 1: Start the Visualizer
      python src/generate_wordCloud.py
      
   Step 2: Initialize the Hash Table
      .\main.exe <window> <kTop> <nDocsVisualize> <capacity> <base> <maxCollision>
         Parameters:(Optional, there are default parameters)
            window: Size of the sliding window (number of docs to keep in memory).
            kTop: Number of top frequent words to export.
            nDocsVisualize: Frequency of output generation (e.g., 1 = every doc).
            capacity: Initial size of the Hash Table.
            base: Base number for the polynomial rolling hash.
            maxCollision: Max collision factor before resizing (0.0 - 1.0).

   Step 3: Start the Normalizer and Lemmatizer and the workflow
      python src/normalize_and_count.py <delay_ms>
         delay_ms:Time in milliseconds to wait between processing documents.