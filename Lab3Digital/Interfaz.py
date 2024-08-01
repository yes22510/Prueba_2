iimport tkinter as tk
from tkinter import font as tkfont, messagebox
import serial
import threading
import time

# Configuración de la ventana principal
root = tk.Tk()
root.title("Control de Potenciómetros y LEDs")
root.geometry("600x550")
root.resizable(False, False)
root.configure(bg='#2c3e50')

# Fuente personalizada
font_conf = {
    "custom": tkfont.Font(family="Arial", size=12, weight="bold"),
    "large": tkfont.Font(family="Arial", size=14, weight="bold"),
    "button": tkfont.Font(family="Arial", size=12, weight="bold")
}

# Función para crear un cuadro con texto estilo tarjeta
def crear_cuadro(root, texto, color_fondo, color_texto):
    frame = tk.Frame(root, bg=color_fondo, bd=2, relief='groove', padx=10, pady=10)
    tk.Label(frame, text=texto, font=font_conf["custom"], bg=color_fondo, fg=color_texto).pack()
    return frame

# Cuadro adicional para mostrar nombre y carnet
crear_cuadro(root, "Autor: Ever Yes\nCarnet: 22510", '#34495e', '#ecf0f1').pack(pady=10, fill=tk.X)

# Configuración del puerto serie
def connect():
    global ser
    try:
        ser = serial.Serial(PORT, BAUDRATE, timeout=1)
        messagebox.showinfo("Éxito", "Conexión exitosa")
        threading.Thread(target=leer_datos_serial, daemon=True).start()
    except serial.SerialException:
        messagebox.showerror("Error", "No se pudo conectar al puerto. Verifica el puerto y vuelve a intentarlo.")

# Función para enviar comandos a los LEDs
def enviar_comando_led():
    try:
        valor = int(led_value_entry.get())
        if 0 <= valor <= 255:
            ser.write(f"n{valor}\n".encode())
            led_value_entry.delete(0, tk.END)
        else:
            raise ValueError("Valor fuera de rango")
    except ValueError as e:
        messagebox.showerror("Error", f"Entrada no válida: {e}")
        led_value_entry.delete(0, tk.END)

# Leer datos del puerto serie
def leer_datos_serial():
    while True:
        if ser and ser.in_waiting > 0:
            try:
                data = ser.readline().decode('utf-8').strip()
                if data.startswith('P1:'):
                    actualizar_texto_cuadro(p1_frame, data)
                elif data.startswith('P2:'):
                    actualizar_texto_cuadro(p2_frame, data)
            except UnicodeDecodeError:
                pass
        time.sleep(0.1)

# Actualizar texto en cuadros de potenciómetros
def actualizar_texto_cuadro(frame, texto):
    frame.winfo_children()[0].config(text=texto)

# Configuración de los controles para los LEDs
led_control_frame = tk.Frame(root, bg='#2c3e50')
led_control_frame.pack(pady=10)

tk.Label(led_control_frame, text="Ingresar valor de LEDs (0-255):", font=font_conf["custom"], bg='#2c3e50', fg='#ecf0f1').grid(row=0, column=0, padx=5)
led_value_entry = tk.Entry(led_control_frame, font=font_conf["large"], width=10, bd=2, relief='solid')
led_value_entry.grid(row=0, column=1, padx=5)

# Configuración de botones
def crear_boton(root, text, comando, color_fondo, color_texto):
    return tk.Button(root, text=text, command=comando, font=font_conf["button"], bg=color_fondo, fg=color_texto, relief='solid', padx=10, pady=5)

button_frame = tk.Frame(root, bg='#2c3e50')
button_frame.pack(pady=10)
crear_boton(button_frame, "Conectar", connect, '#27ae60', '#ecf0f1').grid(row=0, column=0, padx=10, pady=10)
crear_boton(button_frame, "Actualizar LEDs", enviar_comando_led, '#2980b9', '#ecf0f1').grid(row=0, column=1, padx=10, pady=10)
crear_boton(button_frame, "Actualizar Pots", lambda: ser.write(b'1'), '#e67e22', '#ecf0f1').grid(row=0, column=2, padx=10, pady=10)

# Configuración de los cuadros para los potenciómetros
potenciometros_frame = tk.Frame(root, bg='#2c3e50')
potenciometros_frame.pack(pady=20)

p1_frame = crear_cuadro(potenciometros_frame, "P1: 0", '#ecf0f1', '#2c3e50')
p2_frame = crear_cuadro(potenciometros_frame, "P2: 0", '#ecf0f1', '#2c3e50')
p1_frame.grid(row=0, column=0, padx=20, pady=10)
p2_frame.grid(row=0, column=1, padx=20, pady=10)

# Ejecuta la interfaz gráfica
root.mainloop()
