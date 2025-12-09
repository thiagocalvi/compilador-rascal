#!/bin/bash

# Define directories and files
TEST_DIR="testes_rascal_completo"
OUTPUT_FILE="resultados_testes.txt"
COMPILER="./compilador"

# Compile the project first
echo "Compilando o projeto..."
make

if [ $? -ne 0 ]; then
    echo "Erro na compilação. Abortando testes."
    exit 1
fi

# Clear previous results
echo "Rodando testes..." > "$OUTPUT_FILE"
echo "Data: $(date)" >> "$OUTPUT_FILE"
echo "--------------------------------------------------" >> "$OUTPUT_FILE"

# Loop through all .ras files sorted by name
for test_file in $(ls "$TEST_DIR"/*.ras | sort); do
    if [ -f "$test_file" ]; then
        echo "Testando: $test_file"
        echo "==================================================" >> "$OUTPUT_FILE"
        echo "ARQUIVO: $test_file" >> "$OUTPUT_FILE"
        echo "--------------------------------------------------" >> "$OUTPUT_FILE"
        
        # Run the compiler
        # We capture both stdout and stderr
        # We also pass a specific output name for the mepa file to avoid overwriting or using default
        # Using /dev/null for mepa output since we only care about the compiler's stdout/stderr log here
        # Or maybe we should let it generate the mepa file? The user asked for "saidas", usually meaning the console output.
        # But the compiler writes "Código salvo em..." to stdout.
        # Let's just run it.
        $COMPILER "$test_file" /dev/null >> "$OUTPUT_FILE" 2>&1
        
        echo -e "\n" >> "$OUTPUT_FILE"
    fi
done

echo "Testes finalizados. Resultados salvos em $OUTPUT_FILE"
