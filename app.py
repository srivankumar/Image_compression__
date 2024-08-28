import os
from flask import Flask, request, send_file, render_template, jsonify
from werkzeug.utils import secure_filename
import subprocess
import time
from PIL import Image
import io

app = Flask(__name__)
UPLOAD_FOLDER = 'uploads'
COMPRESSED_FOLDER = 'compressed'
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER
app.config['COMPRESSED_FOLDER'] = COMPRESSED_FOLDER

# Ensure folders exist
os.makedirs(UPLOAD_FOLDER, exist_ok=True)
os.makedirs(COMPRESSED_FOLDER, exist_ok=True)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/upload', methods=['POST'])
def upload_file():
    if 'image' not in request.files:
        return jsonify({'error': 'No file part'}), 400

    file = request.files['image']

    if file.filename == '':
        return jsonify({'error': 'No selected file'}), 400

    if file:
        filename = secure_filename(file.filename)
        file_path = os.path.join(app.config['UPLOAD_FOLDER'], filename)
        file.save(file_path)

        compressed_file_path = os.path.join(app.config['COMPRESSED_FOLDER'], filename + ".huff")

        # Call the C program to compress the image
        subprocess.run(['./compress_image', file_path, compressed_file_path], check=True)

        # Convert compressed image to PNG
        with open(compressed_file_path, "rb") as f:
            binary_data = f.read()

        image = Image.open(io.BytesIO(binary_data))
        parameter=55
        time.sleep(5)

        # Save the image to a file
        converted_image_path = os.path.join(app.config['COMPRESSED_FOLDER'], filename + "_recovered.png")
        image.save(file_path,quality=parameter)

        return send_file(file_path)

if __name__ == '__main__':
    app.run(debug=True)
