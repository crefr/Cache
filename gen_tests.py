import random
import os

tests_folder = "tests"

lens = [100, 1000, 10000, 100000]
unique_elem_coeffs = [0.1, 0.2, 0.3, 0.5]
cache_size_coeffs = [0.1, 0.2, 0.3]

for length in lens:
    len_index = 0
    for unique_elems in [int(length * unique_elem_coeffs[i]) for i in range(len(unique_elem_coeffs))]:
        for cache_size in [int(length * cache_size_coeffs[i]) for i in range(len(cache_size_coeffs))]:
            test_name = f"test_{length}_{len_index}.dat"
            len_index += 1

            test_path = os.path.join(tests_folder, test_name)

            with open(test_path, "w") as test_file:
                test_file.write(f"{cache_size} {length}\n")

                for elem_index in range(length):
                    test_file.write(f"{random.randint(1, unique_elems)} ")

