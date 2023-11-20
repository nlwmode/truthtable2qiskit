import os
import sys
from qiskit import QuantumCircuit, Aer, transpile, assemble
from qiskit.visualization import plot_histogram, circuit_drawer

def parse_esop(esop_lines):
    num_inputs = int(esop_lines[0].split()[1])
    num_outputs = int(esop_lines[1].split()[1])
    num_cubes  = int(esop_lines[2].split()[1])
    
    # 创建量子电路
    esop_circuit = QuantumCircuit(num_inputs, num_outputs)

    # Add Toffoli gates dynamically based on ESOP strings
    for line in esop_lines[4:-1]:  # Skip .type esop and .e lines
        line = line.split()
        controls = [i for i, val in enumerate(line[0]) if val == '1']
        target = [i for i, val in enumerate(line[0]) if val == '-']
        
        if len(controls) > 0:
            if len(target) > 0:
                esop_circuit.mcx(controls, target[0])
            elif len(controls) > 1:         # If no '-' (don't care) positions, set target to the last control
                esop_circuit.mcx(controls[:-1], controls[-1])
            elif len(controls) == 1:        # Apply X gate for a single control qubit
                print(controls[0])
                esop_circuit.x( controls[0] )

    esop_circuit.measure_all()

    return esop_circuit


def esop_synth(file_input, file_output):
    with open(file_input, 'r') as file:
        esop_lines = file.readlines()
        # esop_lines = esop_lines.strip().split('\n')
        # 解析 ESOP 并创建量子电路
        esop_circuit = parse_esop(esop_lines)

        # 使用 Aer 模拟器模拟电路
        simulator = Aer.get_backend('aer_simulator')
        compiled_circuit = transpile(esop_circuit, simulator)
        qobj = assemble(compiled_circuit)

        # 运行模拟器
        result = simulator.run(qobj).result()

        # 绘制结果
        counts = result.get_counts(esop_circuit)
        plot_histogram(counts)

        # 绘制图形
        esop_circuit.remove_final_measurements(inplace=True)
        esop_circuit.draw("mpl")
        circuit_drawer(esop_circuit, output='mpl', filename=file_output)



if __name__ == "__main__":
    folder_input  = sys.argv[1]
    
    for filename in os.listdir(folder_input):
        if filename.endswith('.pla'):
            file_input = os.path.join(folder_input, filename)
            file_outut = file_input + ".pdf"
            esop_synth(file_input, file_outut)
        