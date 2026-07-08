# Steganography_project
A lightweight C command-line tool that hides confidential text messages inside BMP/PPM image files using Least Significant Bit (LSB) steganography, with extraction capabilities.

# 🖼️ Image Steganography in C

![C](https://img.shields.io/badge/C-Language-blue?logo=c)
![Status](https://img.shields.io/badge/Status-Completed-success)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-lightgrey)
![License](https://img.shields.io/badge/License-MIT-yellow)

A secure **Image Steganography** application developed in **C** that enables users to hide secret messages inside image files and retrieve them later using a password. The project demonstrates low-level file handling, binary data manipulation, and information hiding techniques while showcasing core C programming concepts.

Unlike encryption, where the existence of the message is visible, **Steganography hides the existence of the message itself**, providing an additional layer of confidentiality.

---

# 📖 Overview

Image Steganography is the process of concealing secret information inside an image without noticeably changing its appearance.

This project uses **Least Significant Bit (LSB) Steganography** to embed text into an image. The hidden message can later be extracted only with the correct password or decoding process.

---

# ✨ Features

- 🔒 Hide secret messages inside an image
- 🔓 Extract hidden messages from an encoded image
- 🛡️ Password-protected message retrieval
- 📁 Supports BMP image format (lossless)
- ⚡ Efficient binary file processing
- 💻 Console-based user interface
- 📂 Lightweight and portable
- 🚀 Written entirely in C

---

# ⚙️ Functionality

## 🔹 Encoding

- Reads the input image.
- Accepts a secret message from the user.
- Accepts a password (if implemented).
- Embeds the message into the image using the **Least Significant Bit (LSB)** technique.
- Generates a new encoded image.

---

## 🔹 Decoding

- Reads the encoded image.
- Validates the password (if implemented).
- Extracts the hidden message.
- Displays the original message.

---

## 🔹 Security

- Hides data without affecting image quality.
- Message remains invisible to normal image viewers.
- Password authentication provides an extra security layer.

---

# 🛠️ Technologies Used

| Technology | Purpose |
|------------|---------|
| C Programming | Core Development |
| File Handling | Read/Write Binary Files |
| Structures | Data Organization |
| Bitwise Operations | LSB Encoding & Decoding |
| Standard C Libraries | Memory and String Operations |
| Command Line Interface | User Interaction |

---



# ⚙️ Installation

Clone the repository

```bash
git clone https://github.com/your-username/Steganography.git
```

Move into the project directory

```bash
cd Steganography
```

Compile the project

```bash
gcc *.c -o steganography
```

or using Makefile

```bash
make
```

---

# 🚀 Usage

## Encode a Message

```bash
./steganography -e input.bmp output.bmp secret.txt
```

---

## Decode a Message

```bash
./steganography -d output.bmp
```

---

# 🔍 How It Works

```
Original BMP Image
        │
        ▼
Read Image Pixels
        │
        ▼
Convert Secret Message into Binary
        │
        ▼
Modify Least Significant Bits
        │
        ▼
Generate Encoded Image
        │
        ▼
Read Encoded Image
        │
        ▼
Extract Binary Data
        │
        ▼
Recover Original Message
```

---

# 🧠 LSB Steganography

The project uses the **Least Significant Bit (LSB)** method.

Each pixel contains color information stored in bytes. By changing only the **least significant bit**, the image looks identical to the human eye while secretly storing data.

Example:

```
Original Byte

11010010

Embed Bit = 1

Modified Byte

11010011
```

Only the last bit changes, making the modification virtually invisible.

---

# ✅ Advantages

- Provides hidden communication.
- Image quality remains nearly unchanged.
- Fast encoding and decoding.
- Lightweight implementation.
- Efficient memory usage.
- Demonstrates bit-level programming.
- Ideal for learning systems programming.

---

# ❌ Disadvantages

- Limited storage capacity.
- Supports only lossless image formats like BMP.
- Image compression may destroy hidden data.
- Not suitable for large files.
- Can be detected using advanced steganalysis tools.
- Less secure if the hidden data is not encrypted.

---

# 🌍 Applications

- Secure communication
- Military and defense
- Digital watermarking
- Copyright protection
- Cybersecurity
- Digital forensics
- Confidential document sharing
- Medical imaging
- Intelligence agencies
- Educational demonstrations

---

# 🌐 Real-Life Use Cases

### 🔒 Secure Communication

Transmit confidential information without revealing that a secret message exists.

### 🏛 Defense & Government

Protect sensitive information during digital communication.

### 📸 Digital Watermarking

Embed ownership information inside digital images.

### 🏥 Healthcare

Securely attach patient information to medical images.

### ⚖️ Digital Forensics

Hide or verify metadata during investigations.

### ☁ Secure Data Sharing

Exchange confidential information through seemingly ordinary images.

---

# 🎯 Biggest Takeaways

- Gained practical experience with **binary file handling**.
- Learned **Least Significant Bit (LSB) Steganography**.
- Improved understanding of **bitwise operations**.
- Strengthened knowledge of **memory management** in C.
- Learned how image files are structured.
- Practiced modular programming using header files.
- Enhanced debugging and problem-solving skills.

---

# 📚 Key Learnings

- C Programming
- File Handling
- Binary File Processing
- Bit Manipulation
- Structures
- Pointers
- Memory Management
- Modular Programming
- Image Processing Fundamentals
- Information Security Basics

---

# 🚀 Future Enhancements

- AES encryption before embedding
- Support for PNG images
- GUI version
- Audio steganography
- Video steganography
- Password hashing
- Multi-file embedding
- Compression before encoding
- Cross-platform support

---

# 📊 Project Highlights

| Category | Details |
|----------|---------|
| Language | C |
| Domain | Cybersecurity |
| Technique | Least Significant Bit (LSB) |
| Image Format | BMP |
| Interface | Command Line |
| Programming Concepts | File Handling, Bitwise Operations, Structures, Pointers |
| Difficulty | Intermediate |

---

# 💡 Why This Project Matters

This project demonstrates how low-level programming concepts in C can be applied to solve real-world cybersecurity problems. It combines binary file handling, bitwise operations, and modular programming to implement a practical information-hiding system, making it an excellent project for students interested in systems programming, cybersecurity, and embedded software development.

---

# 📄 License

This project is licensed under the **MIT License**.

---

# 👨‍💻 Author

**Karan Jeevan**

Final Year Electronics & Communication Engineering Student

**Interests:** Embedded Systems •C Programming • Embedded Software Development



---

## ⭐ If you found this project useful, please consider giving it a star!
