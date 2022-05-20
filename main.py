import subprocess
import matplotlib.pyplot as plt


def main():
    size_list, memory_list = [], []
    for vertex_count in range(10, 15):
        for rep in range(1, 10):
            t1, t2, *rest = \
                subprocess.check_output(
                    ["cmake-build-debug/RandomTriangulation", str(vertex_count)],
                    text=True) \
                .split("\n")
            print(t1, t2)
            t1, t2 = t1.strip(), t2.strip()
            res: str = subprocess.check_output(["cmake-build-debug/Build", t1, t2], text=True)
            flip_distance, time_usage, memory_usage = tuple(map(float, res.split("\n")))
            flip_distance, memory_usage = round(flip_distance), round(memory_usage)
            size_list.append(vertex_count)
            memory_list.append(memory_usage)
    plt.scatter(size_list, memory_list)
    plt.show()


if __name__ == "__main__":
    main()
