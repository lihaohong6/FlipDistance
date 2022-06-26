import dataclasses
import math
import random
import shutil
import subprocess
import sys
from pathlib import Path
from tkinter import Canvas, Tk

import matplotlib.pyplot as plt
import numpy as np
from PIL import Image, EpsImagePlugin


@dataclasses.dataclass()
class Triangulation:
    size: int
    edges: list[tuple[int, int]]
    parentheses: str
    tree: str


def convert_triangulation(s: str) -> Triangulation:
    res: str = subprocess.check_output(["cmake-build-debug/Build", "-c", s], text=True)
    lines = res.split("\n")
    n = int(lines[0])
    result = []
    for i in range(1, n - 2):
        v1, v2 = tuple(map(int, lines[i].split()))
        result.append((v1, v2))
    return Triangulation(n, result, lines[n - 2], s)


def rand_triangulation(n: int) -> tuple[Triangulation, Triangulation]:
    t1, t2, *rest = \
        subprocess.check_output(
            ["cmake-build-debug/RandomTriangulation", str(n)],
            text=True) \
            .split("\n")
    return convert_triangulation(t1.strip()), convert_triangulation(t2.strip())


def run_program(t1: str, t2: str, algo_name: str = "bfs") -> tuple[int, float, int]:
    res: str = subprocess.check_output([f"cmake-build-debug/Build", t1, t2, algo_name], text=True)
    flip_distance, time_usage, memory_usage, *rest = res.split("\n")
    return int(flip_distance), float(time_usage), int(memory_usage)


def run_program_decision(t1: str, t2: str, algo_name: str = "bfs") -> list[tuple[int, bool, float]]:
    res: str = subprocess.check_output([f"cmake-build-debug/Build", t1, t2, algo_name, "1"], text=True)
    result = []
    for index, line in enumerate(res.split("\n")):
        k = index + 1
        ans, time_usage = line.split(" ")
        result.append((k, ans == "1", float(time_usage)))
    return result


def plot_time_usage(start: int, end: int, algo_name: str):
    size_list, plot_list = [], []
    for vertex_count in range(start, end + 1):
        print("===========", vertex_count, "===========")
        for rep in range(1, 20):
            print(rep)
            t1, t2 = rand_triangulation(vertex_count)
            flip_distance, time_usage, memory_usage = run_program(t1.tree, t2.tree, algo_name)
            size_list.append(vertex_count)
            plot_list.append(time_usage)
    plt.scatter(size_list, plot_list)
    y = np.log(np.array(plot_list))
    print(np.polyfit(size_list, y, 1))
    plt.show()


def show_triangulation(canvas: Canvas, t: Triangulation, center_x, center_y, radius):
    delta_angle = 2 * math.pi / t.size
    points: list[tuple[int, int]] = []
    for i in range(t.size):
        angle = -(i + 2) * delta_angle
        y = center_y + math.sin(angle) * radius
        x = center_x + math.cos(angle) * radius
        canvas.create_oval(x - 3, y - 3, x + 3, y + 3, fill="#476042")
        canvas.create_text(x + math.cos(angle) * 25, y + math.sin(angle) * 25, text=str(i), font=DEFAULT_FONT)
        points.append((x, y))
    for e in t.edges:
        canvas.create_line(*points[e[0]], *points[e[1]])
    for p1, p2 in zip([points[-1]] + points, points):
        canvas.create_line(*p1, *p2)
    canvas.create_text(center_x, center_y - radius - 60, text=t.tree, font=DEFAULT_FONT)


DEFAULT_FONT = ("Arial", 36)
TEMP_DIR = "images/temp.ps"


def show_triangulations(t1: Triangulation, t2: Triangulation, text: str, number: int):
    root = Tk()
    width, height = 1920, 1080
    root.geometry("1920x1080")
    canvas = Canvas(root, width=width, height=height)
    radius = width / 4 - 80
    show_triangulation(canvas, t1, width / 4, height / 2, radius)
    show_triangulation(canvas, t2, width / 4 * 3, height / 2, radius)
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
    print("Progress: " + progress * "#" + (char_count - progress) * "-" + " " + f"{current}/{total}", end="\r")


def init_draw():
    p = Path("images")
    shutil.rmtree(p, ignore_errors=True)
    p.mkdir()
    if sys.platform.startswith("win"):
        EpsImagePlugin.gs_windows_binary = r'C:\Program Files\gs\gs9.56.1\bin\gswin64c'


def find_triangulations(n: int, count: int, image: bool, plot: bool):
    init_draw()
    fd_list, mem_list = [], []
    print("Starting...")
    for imageCount in range(count):
        progress_bar(imageCount, count)
        while True:
            t1, t2 = rand_triangulation(n)
            flip_distance, _, mem_usage, *rest = run_program(t1.tree, t2.tree)
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


def get_degrees(t: Triangulation) -> list[int]:
    res = []
    for v in range(t.size):
        res.append(sum([e.count(v) for e in t.edges]))
    return res


def get_highest_degree_vertex(t1: Triangulation, t2: Triangulation) -> tuple[int, int]:
    degree_sums = [v[0] + v[1] for v in zip(get_degrees(t1), get_degrees(t2))]
    max_degree = max(degree_sums)
    return degree_sums.index(max_degree), max_degree


def find_non_trivial_problems(n: int, count: int):
    init_draw()
    for iteration in range(count):
        while True:
            t1, t2 = rand_triangulation(n)
            flip_distance, *rest = run_program(t1.tree, t2.tree)
            # if flip_distance >= 1.2 * (n - 3):
            #     continue
            index, degree = get_highest_degree_vertex(t1, t2)
            message = f"FD: {flip_distance}; Index: {index}; Deg: {degree}; " + \
                      f"Ratio: {(2 * n - 6 - degree) / flip_distance}"
            print(message)
            show_triangulations(t1, t2,
                                message,
                                iteration)
            break


def verify(n: int, count: int, algo1: str, algo2: str):
    init_draw()
    for _ in range(count):
        while True:
            t1, t2 = rand_triangulation(n)
            fd1, time1, *rest = run_program(t1.tree, t2.tree, algo1)
            fd2, time2, *rest = run_program(t1.tree, t2.tree, algo2)
            # if fd2 >= 1.25 * (n - 3):
            #     continue
            print(time1, time2, fd2)
            if not fd1 == fd2:
                message = f"{algo1}: {fd1}; {algo2}: {fd2}"
                print(message)
                show_triangulations(t1, t2, message, random.randint(0, 100))
            break


def main():
    while True:
        t1, t2 = rand_triangulation(15)
        fd, *rest = run_program(t1.tree, t2.tree, "bfs")
        if fd < 18:
            print(fd)
            continue
        print(t1.tree, t2.tree)


if __name__ == "__main__":
    main()
