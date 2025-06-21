import os

def list_text_files(directory):
    try:
        # 获取目录下的所有文件
        files = os.listdir(directory)
        # 过滤出扩展名为.txt的文件
        txt_files = [file for file in files if file.endswith('.txt')]
        
        return txt_files
    except FileNotFoundError:
        print(f"目录 {directory} 不存在。")
        return []
    except Exception as e:
        print(f"发生错误: {e}")
        return []

def main():
    directory = r'd:\data'  # 指定目录
    text_files = list_text_files(directory)

    if text_files:
        print("目录下的文本文件有：")
        for file in text_files:
            print(file)
    else:
        print("没有找到任何文本文件。")

if __name__ == "__main__":
    main()
