import argparse
import subprocess
import sys
from pathlib import Path

RAIZ = Path(__file__).resolve().parent
SRC = RAIZ / "src"
BUILD = RAIZ / "build"
EXECUTAVEL = BUILD / "arvores"

ESTRUTURAS = ["avl"]


def compilar():
    BUILD.mkdir(exist_ok=True)
    fontes = sorted(str(p) for p in SRC.glob("*.c"))
    comando = ["gcc", "-std=c11", "-O2", "-Wall", "-Wextra", "-o", str(EXECUTAVEL), *fontes]
    if subprocess.run(comando).returncode != 0:
        sys.exit(1)


def garantir_compilado():
    if not EXECUTAVEL.exists():
        compilar()
        return
    compilado = EXECUTAVEL.stat().st_mtime
    if any(p.stat().st_mtime > compilado for p in SRC.iterdir()):
        compilar()


def executar(estrutura, semente, tamanhos):
    comando = [str(EXECUTAVEL), estrutura, str(semente), *map(str, tamanhos)]
    resultado = subprocess.run(comando, capture_output=True, text=True)
    if resultado.returncode != 0:
        sys.stderr.write(resultado.stderr)
        sys.exit(1)
    return [tuple(map(int, linha.split(","))) for linha in resultado.stdout.split()]


def cmd_compilar(args):
    compilar()


def cmd_rodar(args):
    garantir_compilado()
    _, ad_busca, ad_ajuste, rm_busca, rm_ajuste = executar(args.estrutura, args.semente, [args.n])[0]
    print(f"{args.estrutura}  n={args.n}  semente={args.semente}")
    print(f"adição   busca={ad_busca}  ajuste={ad_ajuste}  total={ad_busca + ad_ajuste}")
    print(f"remoção  busca={rm_busca}  ajuste={rm_ajuste}  total={rm_busca + rm_ajuste}")


def main():
    parser = argparse.ArgumentParser(description="Esforço de adição e remoção em árvores balanceadas.")
    sub = parser.add_subparsers(dest="comando", required=True)

    p = sub.add_parser("compilar", help="compila o executável em build/")
    p.set_defaults(func=cmd_compilar)

    p = sub.add_parser("rodar", help="roda uma estrutura com um conjunto")
    p.add_argument("estrutura", choices=ESTRUTURAS)
    p.add_argument("--n", type=int, default=1000)
    p.add_argument("--semente", type=int, default=1)
    p.set_defaults(func=cmd_rodar)

    args = parser.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
