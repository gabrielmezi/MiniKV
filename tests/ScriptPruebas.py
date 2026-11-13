
def generar_csv(nombre, cantidad):
    with open(nombre, "w", encoding="utf-8", newline="") as archivo:
        archivo.write("clave,valor,tipo\n")

        for i in range(1, cantidad + 1):
            clave = f"kv{i:06d}"
            valor = f"valor{i:06d}"
            archivo.write(f"{clave},{valor},STRING\n")

    print(f"{nombre}: {cantidad} registros generados")


generar_csv("kv_pequeno.csv", 30)
generar_csv("kv_masivo.csv", 10000)
generar_csv("kv_muy_masivo.csv", 100000)

print("Archivos generados correctamente")
