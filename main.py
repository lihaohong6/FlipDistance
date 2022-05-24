import dataclasses
import math
import shutil
import subprocess
import tkinter
from pathlib import Path
from tkinter import Canvas, Tk
from PIL import ImageGrab, Image, EpsImagePlugin
import numpy as np

import matplotlib.pyplot as plt


def rand_triangulation(n: int) -> tuple[str, str]:
    t1, t2, *rest = \
        subprocess.check_output(
            ["cmake-build-debug/RandomTriangulation", str(n - 2)],
            text=True) \
            .split("\n")
    return t1.strip(), t2.strip()


def run_program(t1: str, t2: str) -> tuple[int, float, int]:
    res: str = subprocess.check_output(["cmake-build-debug/Build", t1, t2], text=True)
    flip_distance, time_usage, memory_usage, *rest = res.split("\n")
    return int(flip_distance), float(time_usage), int(memory_usage)


def plot_memory_usage(start:int, end:int):
    size_list, memory_list = [], []
    for vertex_count in range(start, end + 1):
        print("===========", vertex_count, "===========")
        for rep in range(1, 20):
            print(rep)
            t1, t2 = rand_triangulation(vertex_count)
            flip_distance, time_usage, memory_usage = run_program(t1, t2)
            size_list.append(vertex_count)
            memory_list.append(memory_usage)
    plt.scatter(size_list, memory_list)
    y = np.log(np.array(memory_list))
    print(np.polyfit(size_list, y, 1))
    plt.show()


@dataclasses.dataclass()
class Triangulation:
    size: int
    edges: list[tuple[int, int]]
    parentheses: str
    tree: str


def get_triangulation(s: str) -> Triangulation:
    res: str = subprocess.check_output(["cmake-build-debug/Build", "-c", s], text=True)
    lines = res.split("\n")
    n = int(lines[0])
    result = []
    for i in range(1, n - 2):
        v1, v2 = tuple(map(int, lines[i].split()))
        result.append((v1, v2))
    return Triangulation(n, result, lines[n - 2], s)


def show_triangulation(canvas: Canvas, t: Triangulation, center_x, center_y, radius):
    delta_angle = 2 * math.pi / t.size
    points: list[tuple[int, int]] = []
    for i in range(t.size):
        angle = i * delta_angle
        y = center_y + math.sin(angle) * radius
        x = center_x + math.cos(angle) * radius
        canvas.create_oval(x - 3, y - 3, x + 3, y + 3, fill="#476042")
        points.append((x, y))
    for e in t.edges:
        canvas.create_line(*points[e[0]], *points[e[1]])
    for p1, p2 in zip([points[-1]] + points, points):
        canvas.create_line(*p1, *p2)
    canvas.create_text(center_x, center_y - radius - 40, text=t.tree, font=DEFAULT_FONT)


DEFAULT_FONT = ("Arial", 36)
TEMP_DIR = "images/temp.ps"


def show_triangulations(t1: str, t2: str, text: str, number: int):
    root = Tk()
    width, height = 1920, 1080
    root.geometry("1920x1080")
    canvas = Canvas(root, width=width, height=height)
    radius = width / 4 - 30
    show_triangulation(canvas, get_triangulation(t1), width / 4, height / 2, radius)
    show_triangulation(canvas, get_triangulation(t2), width / 4 * 3, height / 2, radius)
    canvas.create_text(width / 2, height - 50, text=text, font=DEFAULT_FONT)
    canvas.update()
    canvas.postscript(file=TEMP_DIR, width=1920, height=1080)
    img = Image.open(TEMP_DIR)
    img.save("images/image" + str(number) + ".png", "png")
    root.destroy()
    with open("images/data.txt", "a") as f:
        f.write(f"{number}: {t1} {t2}\n")
    img.close()
    Path(TEMP_DIR).unlink(missing_ok=True)


def progress_bar(current, total):
    char_count = 40
    progress = round(current / total * char_count)
    print("Progress: " + progress * "#" + (char_count-progress) * "-" + " " + f"{current}/{total}", end="\r")


def find_triangulations(n: int, count: int, image: bool, plot: bool):
    p = Path("images")
    shutil.rmtree(p, ignore_errors=True)
    p.mkdir()
    EpsImagePlugin.gs_windows_binary = r'C:\Program Files\gs\gs9.56.1\bin\gswin64c'
    fd_list, mem_list = [], []
    print("Starting...")
    for imageCount in range(count):
        progress_bar(imageCount, count)
        while True:
            t1, t2 = rand_triangulation(n)
            flip_distance, _, mem_usage, *rest = run_program(t1, t2)
            fd_list.append(flip_distance)
            mem_list.append(mem_usage)
            if not image:
                break
            elif flip_distance <= (n - 2) * 1.25:
                show_triangulations(t1, t2, str(flip_distance), imageCount)
                break
    if plot:
        y = np.array(mem_list)
        y = np.log(y)
        plt.scatter(fd_list, y)
        print(np.polyfit(fd_list, y, 1))
        plt.show()


def main():
    plot_memory_usage(10, 16)


if __name__ == "__main__":
    main()
