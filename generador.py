import random
import string

def generar_cost_insert(filename):
    with open(filename, 'w') as file:
        costs = [1 if i % 2 != 0 else 2 for i in range(26)]
        file.write(" ".join(map(str, costs)) + "\n")

def generar_cost_delete(filename):
    with open(filename, 'w') as file:
        costs = [1] * 26
        file.write(" ".join(map(str, costs)) + "\n")

def generar_cost_replace(filename):
    with open(filename, 'w') as file:
        costs = [[random.randint(1,2) for _ in range(26)] for _ in range(26)]
        for row in costs:
            file.write(" ".join(map(str, row)) + "\n")

def generar_cost_transpose(filename):
    with open(filename, 'w') as file:
        costs = [[random.randint(1,2) for j in range(26)] for i in range(26)]
        for row in costs:
            file.write(" ".join(map(str, row)) + "\n")

def generate_random_string(length):
    return ''.join(random.choice(string.ascii_lowercase) for _ in range(length))

def generate_repetitive_string(length):
    pattern = ''.join(random.choice(string.ascii_lowercase) for _ in range(3))
    return (pattern * (length // len(pattern) + 1))[:length]

def generate_shifted_string(base, shift):
    return base[-shift:] + base[:-shift]

def save_test_cases(filename, test_cases):
    with open(filename, 'w') as file:
        for s1, s2 in test_cases:
            file.write(f"{s1}\n{s2}\n")
    print(f"Casos de prueba guardados en {filename}")

def generate_identical_cases(length):
    test_cases = []
    if length < 14:
        for i in range(1, length + 1):
            if i > 0:
                s1 = generate_random_string(i)
                test_cases.append((s1, s1))
    else:
        for i in range(1, length + 1, 250):
            if i > 0:
                s1 = generate_random_string(i)
                test_cases.append((s1, s1))
    return test_cases

def generate_substitution_cases(length):
    test_cases = []
    if length < 14:
        for i in range(2, length + 1):  # Empezamos desde 2 para evitar problemas con longitud 1
            s1 = generate_random_string(i)
            s2 = s1[:len(s1) // 2] + random.choice(string.ascii_lowercase) + s1[len(s1) // 2 + 1:]
            test_cases.append((s1, s2))
    else:
        for i in range(2, length + 1, 250):
            s1 = generate_random_string(i)
            s2 = s1[:len(s1) // 2] + random.choice(string.ascii_lowercase) + s1[len(s1) // 2 + 1:]
            test_cases.append((s1, s2))
    return test_cases

def generate_insertion_cases(length):
    test_cases = []
    if length < 14:
        for i in range(1, length + 1):
            if i > 0:
                s1 = generate_random_string(i)
                s2 = s1[:len(s1) // 2] + random.choice(string.ascii_lowercase) + s1[len(s1) // 2:]
                test_cases.append((s1, s2))
    else:
        for i in range(1, length + 1, 250):
            if i > 0:
                s1 = generate_random_string(i)
                s2 = s1[:len(s1) // 2] + random.choice(string.ascii_lowercase) + s1[len(s1) // 2:]
                test_cases.append((s1, s2))
    return test_cases

def generate_deletion_cases(length):
    test_cases = []
    if length < 14:
        for i in range(2, length + 1):
            s1 = generate_random_string(i)
            s2 = s1[:len(s1) // 2] + s1[len(s1) // 2 + 1:]
            test_cases.append((s1, s2))
    else:
        for i in range(2, length + 1, 250):
            s1 = generate_random_string(i)
            s2 = s1[:len(s1) // 2] + s1[len(s1) // 2 + 1:]
            test_cases.append((s1, s2))
    return test_cases

def generate_different_cases(length):
    test_cases = []
    if length < 14:
        for i in range(1, length + 1):
            if i > 0:
                s1 = generate_random_string(i)
                s2 = generate_random_string(i)
                test_cases.append((s1, s2))
    else:
        for i in range(1, length + 1, 250):
            if i > 0:
                s1 = generate_random_string(i)
                s2 = generate_random_string(i)
                test_cases.append((s1, s2))
    return test_cases

def generate_repetitive_cases(length):
    test_cases = []
    if length < 14:
        for i in range(2, length + 1):
            s1 = generate_repetitive_string(i)
            s2 = s1[:len(s1) // 2] + random.choice(string.ascii_lowercase) + s1[len(s1) // 2 + 1:]
            test_cases.append((s1, s2))
    else:
        for i in range(2, length + 1, 250):
            s1 = generate_repetitive_string(i)
            s2 = s1[:len(s1) // 2] + random.choice(string.ascii_lowercase) + s1[len(s1) // 2 + 1:]
            test_cases.append((s1, s2))
    return test_cases

def generate_shifted_cases(length):
    test_cases = []
    if length < 14:
        for i in range(1, length + 1):
            if i > 0:
                s1 = generate_random_string(i)
                shift = min(1, i)
                s2 = generate_shifted_string(s1, shift=shift)
                test_cases.append((s1, s2))
    else:
        for i in range(1, length + 1, 250):
            if i > 0:
                s1 = generate_random_string(i)
                shift = min(5, i)
                s2 = generate_shifted_string(s1, shift=shift)
                test_cases.append((s1, s2))
    return test_cases

def main():
    # Generar los archivos de costos
    generar_cost_insert("cost_insert.txt")
    generar_cost_delete("cost_delete.txt")
    generar_cost_replace("cost_replace.txt")
    generar_cost_transpose("cost_transpose.txt")
    print("Archivos de costos generados con éxito.")

    # Generar casos de prueba para cada tipo (FB y DP)
    length_fb_dp = 13
    save_test_cases("casos_identicos_fb_dp.txt", generate_identical_cases(length_fb_dp))
    save_test_cases("casos_sustitucion_fb_dp.txt", generate_substitution_cases(length_fb_dp))
    save_test_cases("casos_insercion_fb_dp.txt", generate_insertion_cases(length_fb_dp))
    save_test_cases("casos_eliminacion_fb_dp.txt", generate_deletion_cases(length_fb_dp))
    save_test_cases("casos_diferentes_fb_dp.txt", generate_different_cases(length_fb_dp))
    save_test_cases("casos_repetitivos_fb_dp.txt", generate_repetitive_cases(length_fb_dp))
    save_test_cases("casos_desplazados_fb_dp.txt", generate_shifted_cases(length_fb_dp))

    # Generar casos de prueba para cada tipo (solo DP)
    length_dp = 2500
    save_test_cases("casos_identicos_dp.txt", generate_identical_cases(length_dp))
    save_test_cases("casos_sustitucion_dp.txt", generate_substitution_cases(length_dp))
    save_test_cases("casos_insercion_dp.txt", generate_insertion_cases(length_dp))
    save_test_cases("casos_eliminacion_dp.txt", generate_deletion_cases(length_dp))
    save_test_cases("casos_diferentes_dp.txt", generate_different_cases(length_dp))
    save_test_cases("casos_repetitivos_dp.txt", generate_repetitive_cases(length_dp))
    save_test_cases("casos_desplazados_dp.txt", generate_shifted_cases(length_dp))

if __name__ == "__main__":
    main()