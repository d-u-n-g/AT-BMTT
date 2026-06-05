import tkinter as tk
from tkinter import ttk
from tkinter.scrolledtext import ScrolledText

# ================== CỬA SỔ CHÍNH ==================
root = tk.Tk()
root.title("Elgamal Nhóm 3")
root.geometry("1200x700")
root.resizable(False, False)

# ================== KHUNG SINH KHÓA ==================
frame_key = ttk.LabelFrame(root, text="🔑 SINH KHÓA")
frame_key.place(x=10, y=10, width=380, height=660)

ttk.Label(frame_key, text="Chọn độ dài bit:").place(x=10, y=10)
combo_bit = ttk.Combobox(
    frame_key,
    values=["128", "256", "512", "1024", "2048"],
    state="readonly"
)
combo_bit.place(x=10, y=35, width=330)
combo_bit.current(0)

ttk.Label(frame_key, text="q:").place(x=10, y=75)
entry_q = ttk.Entry(frame_key)
entry_q.place(x=10, y=95, width=330)

ttk.Label(frame_key, text="a:").place(x=10, y=135)
entry_g = ttk.Entry(frame_key)
entry_g.place(x=10, y=155, width=330)

ttk.Label(frame_key, text="Xa:").place(x=10, y=195)
entry_xa = ttk.Entry(frame_key)
entry_xa.place(x=10, y=215, width=330)

ttk.Label(frame_key, text="Ya:").place(x=10, y=255)
entry_ya = ttk.Entry(frame_key)
entry_ya.place(x=10, y=275, width=330)

btn_auto = ttk.Button(frame_key, text="Sinh tự động")
btn_auto.place(x=10, y=330, width=330, height=35)

btn_reset = ttk.Button(frame_key, text="Làm mới")
btn_reset.place(x=10, y=375, width=330, height=35)

btn_confirm = ttk.Button(frame_key, text="Xác nhận")
btn_confirm.place(x=10, y=420, width=330, height=35)

# ================== KHUNG MÃ HÓA ==================
frame_encrypt = ttk.LabelFrame(root, text="🔒 MÃ HÓA")
frame_encrypt.place(x=400, y=10, width=380, height=660)

ttk.Label(frame_encrypt, text="Bản rõ:").place(x=10, y=10)

txt_plain = ScrolledText(frame_encrypt)
txt_plain.place(x=10, y=35, width=340, height=120)

btn_open_plain = ttk.Button(frame_encrypt, text="📂 Mở file")
btn_open_plain.place(x=10, y=165, width=340)

ttk.Label(frame_encrypt, text="Xa:").place(x=10, y=210)
entry_enc_xa = ttk.Entry(frame_encrypt)
entry_enc_xa.place(x=10, y=230, width=340)

ttk.Label(frame_encrypt, text="Ya:").place(x=10, y=270)
entry_enc_ya = ttk.Entry(frame_encrypt)
entry_enc_ya.place(x=10, y=290, width=340)

btn_encrypt = ttk.Button(frame_encrypt, text="🔐 Mã hóa")
btn_encrypt.place(x=10, y=340, width=340, height=35)

ttk.Label(frame_encrypt, text="Bản mã:").place(x=10, y=390)

txt_cipher = ScrolledText(frame_encrypt)
txt_cipher.place(x=10, y=415, width=340, height=120)

btn_save_cipher = ttk.Button(frame_encrypt, text="💾 Lưu")
btn_save_cipher.place(x=10, y=545, width=340)

# ================== KHUNG GIẢI MÃ ==================
frame_decrypt = ttk.LabelFrame(root, text="🔓 GIẢI MÃ")
frame_decrypt.place(x=790, y=10, width=380, height=660)

ttk.Label(frame_decrypt, text="Bản mã:").place(x=10, y=10)

txt_cipher_input = ScrolledText(frame_decrypt)
txt_cipher_input.place(x=10, y=35, width=340, height=120)

btn_open_cipher = ttk.Button(frame_decrypt, text="📂 Mở file")
btn_open_cipher.place(x=10, y=165, width=340)

ttk.Label(frame_decrypt, text="Xa:").place(x=10, y=210)
entry_dec_xa = ttk.Entry(frame_decrypt)
entry_dec_xa.place(x=10, y=230, width=340)

btn_decrypt = ttk.Button(frame_decrypt, text="🔓 Giải mã")
btn_decrypt.place(x=10, y=300, width=340, height=35)

ttk.Label(frame_decrypt, text="Bản rõ:").place(x=10, y=360)

txt_plain_output = ScrolledText(frame_decrypt)
txt_plain_output.place(x=10, y=385, width=340, height=120)

btn_save_plain = ttk.Button(frame_decrypt, text="💾 Lưu")
btn_save_plain.place(x=10, y=515, width=340)


root.mainloop()
