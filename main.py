import subprocess
import matplotlib.pyplot as plt


def rand_triangulation(n: int) -> tuple[str, str]:
    t1, t2, *rest = \
        subprocess.check_output(
            ["cmake-build-debug/RandomTriangulation", str(n - 2)],
            text=True) \
            .split("\n")
    return t1.strip(), t2.strip()


def run_prog(t1: str, t2: str) -> tuple[int, float, int]:
    res: str = subprocess.check_output(["cmake-build-debug/Build", t1, t2], text=True)
    flip_distance, time_usage, memory_usage, *rest = res.split("\n")
    return int(flip_distance), float(time_usage), int(memory_usage)


def plot_memory_usage():
    size_list, memory_list = [], []
    start, end = 10, 18
    for vertex_count in range(start, end + 1):
        print("===========", vertex_count, "===========")
        for rep in range(1, 20):
            print(rep)
            t1, t2 = rand_triangulation(vertex_count)
            flip_distance, time_usage, memory_usage = run_prog(t1, t2)
            size_list.append(vertex_count)
            memory_list.append(memory_usage)
    plt.scatter(size_list, memory_list)
    plt.show()


def find_triangulations():
    pass


def main():
    pass


if __name__ == "__main__":
    main()
