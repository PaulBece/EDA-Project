import os
import shutil
import time

PROJECT_ROOT = os.path.dirname(os.path.dirname(__file__))

def clean_folder(folder_path):
    if os.path.exists(folder_path):
        try:
            print(f"Cleanning: {folder_path}", end="")
            shutil.rmtree(folder_path)
            
            time.sleep(0.1) 
            os.makedirs(folder_path)
            print("Done")
        except Exception as e:
            print(f"Error {folder_path}: {e}")
            try:
                for f in os.listdir(folder_path):
                    os.remove(os.path.join(folder_path, f))
            except:
                pass
    else:
        os.makedirs(folder_path)
        print(f"Folder created: {folder_path}")

def main():
    
    folders = [
        os.path.join(PROJECT_ROOT, "pre-processedData", "documents"), 
        os.path.join(PROJECT_ROOT, "output", "documents")             
    ]

    for folder in folders:
        clean_folder(folder)

    print("Done")

if __name__ == "__main__":
    main()