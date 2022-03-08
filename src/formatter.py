import re
from typing import Callable

from src.problem import Problem

RE_CHINESE_CHAR = r"[\u4e00-\u9fa5]"
RE_CHINESE_PUNC = r"[\u3002\uff1b\uff0c\uff1a\u201c\u201d\uff08\uff09\u3001\uff1f\u300a\u300b]"
RE_CHINESE_CHAR_OR_PUNC = r"[\u3002\uff1b\uff0c\uff1a\u201c\u201d\uff08\uff09\u3001\uff1f\u300a\u300b]|[\u4e00-\u9fa5]"

FORMAT_RULES: list[Callable[[str], str]] = [
    lambda s: s.strip(),
    lambda s: re.sub(r" {2}$", "\n", s),  # 双空格 -> 换行符
    lambda s: re.sub(r"\n{3,}", "\n\n", s),  # 删除多余换行符
    lambda s: s.replace('≤', "\\leq"),  # ≤ -> \leq
    lambda s: s.replace('≥', "\\geq"),  # ≥ -> \geq
    lambda s: re.sub(r"\\le\s", lambda m: "\\leq ", s),  # \le -> \leq
    lambda s: re.sub(r"\\ge\s", lambda m: "\\geq ", s),  # \ge -> \geq
    lambda s: re.sub(r"\d{2,}(?=\^)", lambda m: f"{{{m.group()}}}", s),  # 10^5 -> {10}^5
    lambda s: re.sub(rf"(?<={RE_CHINESE_CHAR_OR_PUNC}) (?={RE_CHINESE_CHAR_OR_PUNC})", "", s),  # 删除两汉字之间的空格
    lambda s: re.sub(rf"(?<={RE_CHINESE_CHAR}),(?={RE_CHINESE_CHAR})", "，", s),  # 两汉字间半角逗号 -> 全角
    lambda s: re.sub(rf"(?<={RE_CHINESE_CHAR}):$", "：", s),  # 汉字后半角冒号 -> 全角冒号
    lambda s: re.sub(rf"([A-Za-z0-9$])({RE_CHINESE_CHAR})", lambda m: f"{m.group(1)} {m.group(2)}", s),
    lambda s: re.sub(rf"({RE_CHINESE_CHAR})([A-Za-z0-9$])", lambda m: f"{m.group(1)} {m.group(2)}", s),  # 增加汉英间空格
    lambda s: re.sub(r"^- ", "* ", s),  # - -> *
    lambda s: re.sub(r"(?<=[a-z0-9])\\", " \\", s),  # 增加公式中反斜杠前空格
]


class Formatter(Problem):
    def __init__(self, pid: str):
        super().__init__(pid)

    @staticmethod
    def __format_line(line: str):
        for rule in FORMAT_RULES:
            line = rule(line)
        return line

    def format(self):
        with open(self._path_md, "r+", encoding="UTF-8") as fp:
            lines = list(
                map(self.__format_line,
                    map(lambda r: r.strip(),
                        filter(lambda s: s != '\n', fp.readlines())
                        )
                    )
            )

        with open(self._path_md, "w", encoding="UTF-8") as fp:
            sample = False
            for i, line in enumerate(lines):
                if re.match(r"```(input|output)\d+", line):
                    sample = True
                elif sample and line == "```":
                    sample = False
                fp.write(f"{line}\n")
                if not (sample or i == len(lines) - 1):
                    fp.write("\n")
