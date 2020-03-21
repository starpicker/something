import pyzbar.pyzbar
from PIL import Image
import sys

#barcodes = pyzbar.pyzbar.decode(Image.open(sys.argv[1]), symbols = [pyzbar.ZBarSymbol.QRCODE])
barcodes = pyzbar.pyzbar.decode(Image.open(sys.argv[1]))
for barcode in barcodes:
    barcodeData = barcode.data.decode("utf-8")
    print(barcodeData)
