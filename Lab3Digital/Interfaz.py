import tkinter as tk
from tkinter import font as tkfont, messagebox
import serial
import threading
import time

# Configuración del puerto serial
PORT = 'COM4'  # Ajusta esto según el puerto de tu dispositivo
BAUDRATE = 9600
ser = None

# Configuración de la ventana principal
root = tk.Tk()
root.title("Control de Potenciómetros y LEDs")
root.geometry("600x550")
root.resizable(False, False)
root.configure(bg='#2c3e50')

# Fuente personalizada
custom_font = tkfont.Font(family="Arial", size=12, weight="bold")
large_font = tkfont.Font(family="Arial", size=14, weight="bold")
button_font = tkfont.Font(family="Arial", size=12, weight="bold")

# Función para crear un cuadro con texto estilo tarjeta
def crear_cuadro(root, texto, color_fondo, color_texto):
    frame = tk.Frame(root, bg=color_fondo, bd=2, relief='groove', padx=10, pady=10)
    label = tk.Label(frame, text=texto, font=custom_font, bg=color_fondo, fg=color_texto)
    label.pack()
    return frame

# Cuadro adicional para mostrar nombre y carnet
info_frame = crear_cuadro(root, "Autor: Ever Yes\nCarnet: 22510", '#34495e', '#ecf0f1')
info_frame.pack(pady=10, fill=tk.X)

# Función para conectar con el puerto serie
def connect():
    global ser
    try:
        ser = serial.Serial(PORT, BAUDRATE, timeout=1)
        messagebox.showinfo("Éxito", "Conexión exitosa")
        iniciar_thread()
    except serial.SerialException:
        messagebox.showerror("Error", "No se pudo conectar al puerto. Verifica el puerto y vuelve a intentarlo.")

# Configuración de los controles para los LEDs
led_control_frame = tk.Frame(root, bg='#2c3e50')
led_control_frame.pack(pady=10)

led_label = tk.Label(led_control_frame, text="Ingresar valor de LEDs (0-255):", font=custom_font, bg='#2c3e50', fg='#ecf0f1')
led_label.grid(row=0, column=0, padx=5)

led_value_entry = tk.Entry(led_control_frame, font=large_font, width=10, bd=2, relief='solid')
led_value_entry.grid(row=0, column=1, padx=5)

# Funciones de actualización
def enviar_comando_led():
    if ser and ser.is_open:
        try:
            valor = int(led_value_entry.get())
            if 0 <= valor <= 255:
                ser.write(f"n{valor}\n".encode())
                led_value_entry.delete(0, tk.END)  # Limpia el campo de entrada
            else:
                messagebox.showerror("Error", "Valor fuera de rango (0-255)")
                led_value_entry.delete(0, tk.END)  # Limpia el campo de entrada
        except ValueError:
            messagebox.showerror("Error", "Entrada no válida")
            led_value_entry.delete(0, tk.END)  # Limpia el campo de entrada

def actualizar_potenciometros(pot_id):
    if ser and ser.is_open:
        if pot_id == 1:
            ser.write(b'1')
        elif pot_id == 2:
            ser.write(b'2')

def leer_datos_serial():
    while True:
        if ser and ser.in_waiting > 0:
            try:
                data = ser.readline().decode('utf-8').strip()
                if data.startswith('P1:'):
                    p1_value = data.split(":")[1].strip()
                    root.after(0, lambda: actualizar_texto_cuadro(p1_frame, f"P1: {p1_value}"))
                elif data.startswith('P2:'):
                    p2_value = data.split(":")[1].strip()
                    root.after(0, lambda: actualizar_texto_cuadro(p2_frame, f"P2: {p2_value}"))
            except UnicodeDecodeError:
                pass
        time.sleep(0.1)  # Pausa para reducir el uso del CPU

def actualizar_texto_cuadro(frame, texto):
    label = frame.winfo_children()[0]
    label.config(text=texto)

# Configuración de los botones
def crear_boton(root, text, comando, color_fondo, color_texto):
    boton = tk.Button(root, text=text, command=comando, font=button_font, bg=color_fondo, fg=color_texto, relief='solid', padx=10, pady=5)
    return boton

# Crear un marco para los botones
button_frame = tk.Frame(root, bg='#2c3e50')
button_frame.pack(pady=10)

# Crear los botones
connect_button = crear_boton(button_frame, "Conectar", connect, '#27ae60', '#ecf0f1')
send_led_button = crear_boton(button_frame, "Actualizar LEDs", enviar_comando_led, '#2980b9', '#ecf0f1')
update_pots_button = crear_boton(button_frame, "Actualizar Pots", lambda: actualizar_potenciometros(1), '#e67e22', '#ecf0f1')

connect_button.grid(row=0, column=0, padx=10, pady=10)
send_led_button.grid(row=0, column=1, padx=10, pady=10)
update_pots_button.grid(row=0, column=2, padx=10, pady=10)

# Configuración de los cuadros para los potenciómetros
potenciometros_frame = tk.Frame(root, bg='#2c3e50')
potenciometros_frame.pack(pady=20)

p1_frame = crear_cuadro(potenciometros_frame, "P1: 0", '#ecf0f1', '#2c3e50')
p2_frame = crear_cuadro(potenciometros_frame, "P2: 0", '#ecf0f1', '#2c3e50')

p1_frame.grid(row=0, column=0, padx=20, pady=10)
p2_frame.grid(row=0, column=1, padx=20, pady=10)

# Inicia el hilo para leer el puerto serial
def iniciar_thread():
    thread = threading.Thread(target=leer_datos_serial, daemon=True)
    thread.start()

# Ejecuta la interfaz gráfica
root.mainloop()
