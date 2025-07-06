from flask import Flask, request, jsonify
import subprocess
import json
import traceback

from flask_cors import CORS

app = Flask(__name__)
CORS(app)  # Habilita CORS para todas las rutas

@app.route('/generate_suffix_array', methods=['POST'])
def generate_suffix_array():
    try:
        data = request.get_json()
        print("Data recibida:", data)
        if not data or 'text' not in data:
            return jsonify({'error': 'No se recibio texto'}), 400

        text = data['text']
        print("Texto recibido:", text)

        # Ejecutar el programa C++ pasando la palabra
        result = subprocess.run(
            ['./output/suffix_array.exe', text],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=5
        )
        print("Subprocess returncode:", result.returncode)
        print("Subprocess stdout:", result.stdout)
        print("Subprocess stderr:", result.stderr)

        if result.returncode != 0:
            return jsonify({'error': 'Error en ejecucion del programa', 'details': result.stderr}), 500

        # Intentar parsear el JSON que devuelve el programa C++
        suffix_array_json = json.loads(result.stdout)
        print("JSON parseado:", suffix_array_json)

        return jsonify(suffix_array_json)

    except Exception as e:
        traceback.print_exc()
        return jsonify({'error': 'Error interno del servidor', 'details': str(e)}), 500

if __name__ == '__main__':
    app.run(debug=True)
