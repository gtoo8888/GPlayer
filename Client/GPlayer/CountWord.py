import os
import re


class FileInfo():
    def __init__(self, type=""):
        self.type = type
        self.file_count = 0
        self.file_line = 0

    def __str__(self):
        return "{:5s} file: count: {:2d}, line: {:4d}".format(self.type, self.file_count, self.file_line)

    def __add__(self, other):
        fi = FileInfo(self.type)
        fi.file_count = self.file_count + other.file_count
        fi.file_line = self.file_line + other.file_line
        return fi


class CountWord():
    def __init__(self):
        self.fi_h = FileInfo("h")
        self.fi_cpp = FileInfo("cpp")
        self.fi_ui = FileInfo("ui")
        self.fi_error = FileInfo("error")
        self.fi_sum = FileInfo("sum")
        self.folder_names = ["src", "include", "test", "ui"]

    def count_lines_without_comments(self, file_path, blank_line=True):
        try:
            with open(file_path, 'r', encoding='utf-8') as file:
                lines = file.readlines()
                if blank_line:
                    lines = [line.strip() for line in lines if not line.strip().startswith('//')]
            return len(lines)
        except:
            self.fi_error.file_count += 1
            print("Error!:{}".format(file_path))
            return 0

    def traverse_directory(self, parent_folder, blank_line=True):
        for folder_name in self.folder_names:
            folder_path = os.path.join(parent_folder, folder_name)
            if os.path.exists(folder_path) and os.path.isdir(folder_path):
                self.process_files_in_folder(folder_path, blank_line)

    def process_files_in_folder(self, folder_path, blank_line=True):
        for file_name in os.listdir(folder_path):
            file_path = os.path.join(folder_path, file_name)
            if os.path.isdir(file_path):
                self.traverse_directory(file_path, blank_line)
            elif file_name.endswith('.h'):
                self.fi_h.file_count += 1
                lines_count = self.count_lines_without_comments(file_path, blank_line)
                self.fi_h.file_line += lines_count
                print("{}: {} lines".format(file_path, lines_count))
            elif file_name.endswith('.cpp'):
                self.fi_cpp.file_count += 1
                lines_count = self.count_lines_without_comments(file_path, blank_line)
                self.fi_cpp.file_line += lines_count
                print("{}: {} lines".format(file_path, lines_count))
            elif file_name.endswith('.ui'):
                self.fi_ui.file_count += 1

    def summary(self, parent_folder, blank_line=True):
        self.traverse_directory(parent_folder, blank_line)
        self.fi_sum += self.fi_h + self.fi_cpp + self.fi_ui

    def __str__(self):
        str_ans = "{:s}\n{:s}\n{:s}\n{:s} ---\n{:s}". \
            format(str(self.fi_h), str(self.fi_cpp), str(self.fi_ui), str(self.fi_error), str(self.fi_sum))
        return str_ans


if __name__ == "__main__":
    folder_path = "."
    count_word = CountWord()
    count_word.summary(folder_path, blank_line=True)
    print(count_word)
