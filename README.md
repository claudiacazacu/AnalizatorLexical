# Analizator Lexical 

## Descriere
Acest proiect implementează un **analizator lexical** pentru un limbaj de programare de tip **Rust**, realizat în cadrul disciplinei **Compilatoare**.

Analizatorul este scris în **C++** și are rolul de a parcurge un fișier sursă de intrare (`.rs`) și de a identifica **tokenii** care apar în acesta, clasificându-i în funcție de tipul lor (cuvinte cheie, identificatori, operatori, delimitatori, constante, comentarii etc.).

---

## Obiectivul proiectului
Scopul proiectului este realizarea unui analizator lexical care:
- citește un fișier sursă de intrare;
- identifică tokenii conform regulilor limbajului;
- determină pentru fiecare token:
  - lexema,
  - tipul tokenului,
  - lungimea lexemei,
  - linia pe care apare;
- semnalează erorile lexicale fără a opri analiza.

---

## Tipuri de tokeni recunoscuți
Analizatorul lexical recunoaște următoarele categorii de tokeni:

- **Cuvinte cheie (keywords)** – specifice limbajului Rust (`fn`, `let`, `if`, `while`, `return`, etc.)
- **Identificatori** – nume de variabile și funcții
- **Constante întregi** (`int_constant`)
- **Constante reale** (`float_constant`)
- **Literali de șir** (`string_literal`)
- **Operatori** (`+`, `-`, `*`, `/`, `%`, `==`, `!=`, `<=`, `>=`, etc.)
- **Delimitatori** (`(`, `)`, `{`, `}`, `;`, `,`, etc.)
- **Comentarii**
  - comentarii pe o linie (`//`)
  - comentarii pe mai multe linii (`/* ... */`)
- **Erori lexicale**
  - caractere necunoscute
  - string-uri neînchise
  - comentarii neînchise

---

## Modul de funcționare
Analizatorul funcționează pe baza unei funcții de scanare (`iaurmatorul`) care:
1. Sare peste spații, tab-uri și linii noi.
2. Identifică tokenul curent pornind de la primul caracter semnificativ.
3. Determină cel mai lung șir valid care formează un token.
4. Returnează informațiile asociate tokenului.
5. În cazul unei erori lexicale, afișează un mesaj de eroare și continuă analiza.

Procesul se repetă până la atingerea sfârșitului fișierului.

---
