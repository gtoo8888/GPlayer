import os
import glob
import csv
import re
from typing import List, Dict, Optional
from pathlib import Path
from collections import defaultdict


class CodeInfo:
    __slots__ = ['file_name', 'file_type', 'line_count', 'classify', 'relative_path']

    def __init__(self, file_name="", file_type="", line_count=0, classify="", relative_path=""):
        self.file_name = file_name
        self.file_type = file_type
        self.line_count = line_count
        self.classify = classify
        self.relative_path = relative_path

    def get_csv_item(self):
        return [self.file_name, self.file_type, self.line_count, self.classify, self.relative_path]

    def get_markdown_item(self):
        return f"| {self.file_name} | {self.file_type} | {self.line_count} | {self.classify} | {self.relative_path} |\n"


FILTERED_FILES = ['UI*', "__init__.py",
                  "Chat1.md", "cJSON.c"
                  ]
FILTERED_DIRS = ['webbench-1.5', "UnitTest*", 'build',
                 "brandy", "ori_compile", "coroutine_lib_gtoo", "new_src",
                 "dbow3", "third_ros*", "pcl_learn*"]
EXTENSIONS = {
    '*.c': 'C',
    '*.cc': 'C++',
    '*.cxx': 'C++',
    '*.cpp': 'C++',
    '*.h': 'C++ Header',
    '*.h++': 'C++ Header',
    '*.hpp': 'C++ Header',
    '*.py': 'Python',
    '*.sh': 'Script',
    # '*.md': 'Markdown',
    # 'CMake*': 'Build',
    # 'Make*': 'build',
    '.gitlab-ci.yml': 'CI'
}


class GetCodeInfo:
    def __init__(self, input_dir: Optional[str] = ""):
        self.output_md_path: str = "code_stats.md"
        self.output_csv_path: str = "code_stats.csv"
        self.filtered_files: List[str] = FILTERED_FILES
        self.filtered_dirs: List[str] = FILTERED_DIRS
        self.extensions: Dict[str, str] = EXTENSIONS
        self.input_dir: Path = Path(input_dir) if input_dir else Path.cwd()
        self.filename: str = self.input_dir.name
        self.total_files: int = 0
        self.total_lines: int = 0
        self.code_stats_list: List[Dict[str, int]] = []
        self.file_type_counts: Dict[str, int] = defaultdict(int)  # 用于统计不同类型的文件数量

    def count_lines_in_file(self, file_path):
        with open(file_path, 'rb') as f:
            return sum(1 for line in f if line.strip())

    def is_filtered_dir(self, file_path, filtered_dirs):
        relative_path = os.path.relpath(file_path, self.input_dir)
        return any(re.match(f, relative_path) for f in filtered_dirs)

    def get_code_statistics(self, directory='.') -> None:
        if self.input_dir == "":
            self.input_dir = directory
        for pattern, file_type in self.extensions.items():
            for file_path in glob.iglob(os.path.join(self.input_dir, '**', pattern), recursive=True):
                file_name = os.path.basename(file_path)
                if any(re.match(f, file_name) for f in self.filtered_files):
                    continue

                if self.is_filtered_dir(file_path, self.filtered_dirs):
                    continue

                line_count = self.count_lines_in_file(file_path)
                relative_path = os.path.relpath(file_path, self.input_dir)
                classify = os.path.split(relative_path)[0].split("\\")[0]
                code_info = CodeInfo(file_name, file_type, line_count, classify, relative_path)
                self.code_stats_list.append(code_info)
                self.total_files += 1
                self.total_lines += line_count
                self.file_type_counts[file_type] += 1

        # self.code_stats_list.sort(key=lambda x: x.file_name, reverse=True)
        self.code_stats_list.sort(key=lambda x: x.line_count, reverse=True)

    def print_and_write_markdown_table(self) -> None:
        markdown_content = f"**{self.filename} code statistics**\n"
        markdown_content += "| file name | type | line count | file classify | file path |\n"
        markdown_content += "| --- | --- | --- | --- | --- |\n"
        for idx, cstats in enumerate(self.code_stats_list):
            markdown_content += cstats.get_markdown_item()
        markdown_content += f"| **sum** | | **{self.total_lines}** | |\n"

        with open(self.output_md_path, encoding="utf-8", mode='w') as f:
            f.write(markdown_content)
        print("markdown_save ok")

    def write_csv_table(self) -> None:
        with open(self.output_csv_path, encoding="gbk", mode='w', newline='') as file:
            writer = csv.writer(file)
            writer.writerow(['file name', 'type', 'line count', 'file classify', 'file path'])
            for idx, cstats in enumerate(self.code_stats_list):
                writer.writerow(cstats.get_csv_item())
            writer.writerow(['sum', '', self.total_lines, ''])
        print("csv save ok")

    # 和上面统计不一致
    def print_summary(self) -> None:
        headers = [f"{self.filename}", "*.c", "*.cc", "*.cxx", "*.cpp", "*.h", "*.h++", "*.hpp", '*.py',
                   '*.sh', '*.md', "**总和**"]

        file_type_line_counts = defaultdict(int)
        for code_info in self.code_stats_list:
            file_type_line_counts[code_info.file_type] += code_info.line_count

        counts = [self.file_type_counts.get(ext, 0) for ext in
                  ["C", "C++", "C++", "C++", "C/C++ Header", "C++ Header", "C++ Header", 'Python', 'Script',
                   'Markdown']]
        line_counts = [file_type_line_counts.get(ext, 0) for ext in
                       ["C", "C++", "C++", "C++", "C/C++ Header", "C++ Header", "C++ Header", 'Python', 'Script',
                        'Markdown']]
        total_files = sum(counts)
        total_lines = sum(line_counts)

        markdown_table = "| " + " | ".join(headers) + " |\n"
        markdown_table += "|" + "|".join(["---"] * len(headers)) + "|\n"
        markdown_table += f"| {total_files} | " + " | ".join(map(str, counts)) + f" | **{total_files}** |\n"
        markdown_table += f"| {total_lines} | " + " | ".join(map(str, line_counts)) + f" | **{total_lines}** |\n"

        print(markdown_table)
        with open(self.output_md_path, encoding="utf-8", mode='a') as f:
            f.write(markdown_table)


# TODO 输出总结
if __name__ == "__main__":
    code_dir = r"/date_sdb/soft/0_my_test/7_ffmpeg/GPlayer/Server/GPlayerServer"

    get_code_info = GetCodeInfo(code_dir)
    get_code_info.get_code_statistics()
    get_code_info.print_and_write_markdown_table()
    get_code_info.write_csv_table()
