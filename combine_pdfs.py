import os
import sys
import math
import fitz  # PyMuPDF
from PIL import Image

def convert_pdfs_to_image(pdf_folder, output_image_path, cols):
    # Get a list of all PDF files in the folder
    pdf_files = [file for file in os.listdir(pdf_folder) if file.endswith('.pdf')]

    # Create a list to store images
    images = []

    # Convert each PDF to images
    for pdf_file in pdf_files:
        pdf_path = os.path.join(pdf_folder, pdf_file)
        pdf_document = fitz.open(pdf_path)

        # Create a list to store page images
        page_images = []

        # Convert each page to an image
        for page_number in range(pdf_document.page_count):
            page = pdf_document[page_number]
            image = page.get_pixmap()
            page_images.append(Image.frombytes("RGB", [image.width, image.height], image.samples))

        # Close the PDF document
        pdf_document.close()

        # Append the page images to the main images list
        images.extend(page_images)

    # Determine the size of the final image
    width, height = images[0].size
    rows = math.ceil(len(images) / cols)
    final_image = Image.new("RGB", (width * cols, height * rows), color="white")

    # Paste each image into the final image
    for i, image in enumerate(images):
        row = i // cols
        col = i % cols
        final_image.paste(image, (col * width, row * height))

    # Save the final image
    final_image.save(output_image_path)

if __name__ == "__main__":
    # Specify the input PDF folder and output image file
    input_pdf_folder = sys.argv[1]
    output_image_file = "maj3s_qiskit.png"

    # Convert PDFs to image and save
    convert_pdfs_to_image(input_pdf_folder, output_image_file, 4)