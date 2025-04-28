from flask import Flask, render_template
import sqlite3

# funçaõ para gerar banco de dados caso não exista
def inicializar_banco():
    conn = sqlite3.connect('/firmware/data/acesso.db')
    cursor = conn.cursor()

    # Cria tabela de usuários
    cursor.execute("""
        CREATE TABLE IF NOT EXISTS usuarios (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            nome TEXT NOT NULL UNIQUE,
            senha TEXT NOT NULL,
            admin INTEGER NOT NULL
        );
    """)

    # Cria tabela de eventos
    cursor.execute("""
        CREATE TABLE IF NOT EXISTS eventos (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            usuario TEXT NOT NULL,
            porta INTEGER NOT NULL,
            acao TEXT NOT NULL,
            data_hora DATETIME DEFAULT CURRENT_TIMESTAMP
        );
    """)

    conn.commit()
    conn.close()

# inicializando db
inicializar_banco()

# inicializando server
app = Flask(__name__)

@app.route("/")
def eventos():
    conn = sqlite3.connect('/firmware/data/acesso.db')
    cursor = conn.cursor()
    cursor.execute("""
        SELECT id, usuario, porta, acao, data_hora from eventos;
    """)
    eventos = cursor.fetchall()
    conn.close()
    return render_template('eventos.html', eventos=eventos)

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=5000)