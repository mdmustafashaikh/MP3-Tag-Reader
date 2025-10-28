## 🎵 MP3 Tag Reader and Editor in C

### 🧩 Project Overview

The **MP3 Tag Reader and Editor** is a C-based command-line tool designed to **read, edit, and display ID3 tags** in MP3 audio files. It helps users extract and modify metadata such as **Title, Artist, Album, Year, Genre, and Comment**, providing hands-on experience with **file parsing**, **binary data manipulation**, and **command-line argument handling** in C.

---

### 🚀 Features

* 📖 **Read** and display ID3v1 tags from MP3 files
* ✏️ **Edit** specific tag fields like Title, Artist, or Album
* 💾 **Save** changes directly back to the MP3 file
* ⚙️ Uses **file I/O** and **structure-based parsing**
* 🧠 Demonstrates practical **binary data handling** in C

---

### ⚙️ Technologies Used

* **Language:** C
* **Concepts:** File I/O, Structures, Command-line arguments, Binary parsing
* **Platform:** Linux / Windows (GCC Compiler)

---

### 🧪 Example Usage

```bash
# To view tags
./mp3tag -v song.mp3

# To edit tags
./mp3tag -e -t "New Title" -a "New Artist" -A "New Album" song.mp3
```

---

### 📄 Learning Outcomes

* Understand **ID3 tag structure** and **MP3 metadata handling**
* Gain experience in **binary file parsing**
* Learn **CLI-based user interface** design
* Practice **modular C programming**

---

### 📚 Future Enhancements

* Add support for **ID3v2 tags**
* Implement **batch editing** for multiple MP3 files
* Provide a **GUI version** for easier metadata management
