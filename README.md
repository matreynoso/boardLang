[![✗](https://github.com/matreynoso/boardLang/actions/workflows/ci.yaml/badge.svg?branch=development)](https://github.com/matreynoso/boardLang/actions/workflows/ci.yaml)

# boardLang

Un lenguaje declarativo para describir juegos de mesa por turnos, de tablero y con piezas, junto con su compilador, desarrollado en C con Flex y Bison sobre el proyecto base [Flex-Bison-Compiler](https://github.com/Alpha-Theta-Gamma-Mu/Flex-Bison-Compiler).

Permite describir, por ejemplo, un ajedrez reducido (con jaque mate, promoción y doble paso del peón, pero sin enroque ni captura al paso), variantes con tableros irregulares o toroidales, juegos de carrera hacia una meta, de captura de la bandera o de combate con piezas que tienen vida y atacan a distancia.

Proyecto Especial de _72.39 - Autómatas, Teoría de Lenguajes y Compiladores_ (ITBA).

* [Equipo](#equipo)
* [Estado](#estado)
* [El lenguaje](#el-lenguaje)
* [Requisitos](#requisitos)
* [Configuración](#configuración)
* [Comandos](#comandos)
* [Casos de prueba](#casos-de-prueba)
* [CI/CD](#cicd)
* [Extensiones recomendadas](#extensiones-recomendadas)

## Equipo

**Grupo G-154**

| Integrante              | Legajo |
| :---------------------- | :----: |
| Matías Reynoso Fandiño  | 61402  |
| Santiago Patiño         | 65549  |
| Juan Ferrin             | 65135  |
| Felipe Merlo            | 65736  |

## Estado

**Stage II (frontend):** el analizador léxico (Flex) y el sintáctico (Bison) están completos: dado un programa, el compilador construye su AST (_Abstract Syntax Tree_), o lo rechaza si tiene errores léxicos o sintácticos.

Todavía no hay análisis semántico ni generación de código (Stage III). Por eso, los programas sintácticamente válidos pero semánticamente incorrectos (por ejemplo, una pieza que se usa sin haber sido declarada) se aceptan por ahora: son los _falsos positivos_ que anticipa el enunciado. Esos casos ya están escritos en `src/test/c/ignore/` (ver [Casos de prueba](#casos-de-prueba)).

## El lenguaje

Un programa describe un juego con tres tipos de bloques, que pueden aparecer en cualquier orden:

* **`game`** (exactamente uno): el tablero (`board`), los casilleros bloqueados (`blocked`), los bordes (`edges: bounded | wrap`), el orden cíclico de los turnos (`cycle`), qué acciones componen un turno (`turn`) y la condición de victoria (`win`).
* **`piece`** (cero o más): un tipo de pieza, con cómo se mueve (`moves`), cómo ataca sin moverse (`attacks`), su vida (`health`), si es real (`royal`), a qué se promociona (`promote`) y cuánto vale (`value`).
* **`player`** (uno por participante): su punto de vista (`pov: N | S | E | W`), su meta opcional (`goal`) y la ubicación inicial de sus piezas.

Los movimientos se componen con tres operadores, de mayor a menor precedencia: `|` (conjuntos de distancias, orientaciones o bases dentro de un paso), `&` (pasos encadenados en un único movimiento) y `,` (alternativas). Cada alternativa lleva sus propios modificadores al final: el modo de reemplazo (`cant`, `can` o `must replace`) y `first move` en `moves`, y `damage` en `attacks`. Las posiciones se escriben estilo ajedrez, con una o dos letras para la columna (`a1`, `h8`, `aa2`).

Por ejemplo, un ajedrez reducido:

```
// Ajedrez reducido: se gana por jaque mate (el rey es "royal").
game {
  board: 8 x 8
  cycle: blanco negro
  win: capture 1 rey
}

piece peon {
  moves: 1 front straight, 2 only front straight first move, 1 front diagonal must replace
  promote: dama at last row
}

piece dama {
  moves: inf (straight|diagonal) can replace
}

piece rey {
  moves: 1 (straight|diagonal) can replace
  royal
}

player blanco {
  rey at e1
  dama at d1
  peon at d2, e2
}

player negro {
  pov: N
  rey at e8
  dama at d8
  peon at d7, e7
}
```

Hay más ejemplos, uno por construcción, en `src/test/c/accept/`.

## Requisitos

* [Docker v29.7.2](https://www.docker.com/)

## Configuración

Las siguientes variables de entorno controlan el comportamiento del compilador:

| Nombre                | Default | Descripción                                                                                                                                                       |
| :-------------------- | :-----: | :---------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `ENVIRONMENT`         | `Local` | El nombre del entorno activo. Los entornos disponibles son `Local`, `Development` y `Production`.                                                                 |
| `LOG_IGNORED_LEXEMES` | `true`  | Si es `true`, registra en el log (nivel `DEBUGGING`) los lexemas ignorados por Flex (blancos y comentarios). Con `false` se quitan esos registros de la consola.   |
| `LOGGING_LEVEL`       | `ALL`   | El nivel mínimo de log que se muestra en la consola. De menor a mayor: `ALL`, `DEBUGGING`, `INFORMATION`, `WARNING`, `ERROR` y `CRITICAL`.                        |
| `PRINT_AST`           | `false` | Si es `true`, imprime en la salida estándar el AST de cada programa aceptado, como un árbol indentado. Combinada con `LOGGING_LEVEL=ERROR` se ve solo el árbol.   |

_Docker Compose_ también puede leer las variables desde un archivo `.env` (ver `compose.yaml`).

## Comandos

### Iniciar

Levanta un contenedor efímero, listo para desarrollar. El resto de los comandos se ejecutan dentro de él:

```bash
docker compose run --rm compiler
```

### Compilar

Compila (o recompila) el compilador completo:

```bash
src/main/bash/build.sh
```

### Ejecutar

Compila un programa:

```bash
src/main/bash/run.sh <programa>
```

donde `<programa>` es la ruta al archivo fuente (por convención, con extensión `.itba`). El código de salida es `0` si el programa se acepta, y distinto de `0` si se rechaza.

### Ver el AST

Para ver el árbol que construye el parser:

```bash
PRINT_AST=true LOGGING_LEVEL=ERROR src/main/bash/run.sh src/test/c/accept/20-reduced-chess.itba
```

En el árbol, lo que el programa no escribió aparece como `unspecified` (los valores por defecto se aplicarán en el análisis semántico), y cada posición muestra su coordenada decodificada como `columna:fila` (por ejemplo, `aa2 (27:2)`).

### Tests

Ejecuta todos los casos de prueba de `src/test/c`:

```bash
src/main/bash/test.sh
```

### Detener

Sale del contenedor y elimina los contenedores efímeros:

```bash
exit
docker compose down
```

### Docker

| Comando                                 | Descripción                                                 |
| :-------------------------------------- | :---------------------------------------------------------- |
| `docker builder prune --all`            | Elimina todos los builds y la caché de builds.              |
| `docker compose --progress=plain build` | Fuerza el build (o rebuild) de las imágenes del cluster.    |
| `docker image prune`                    | Elimina las imágenes huérfanas de Docker.                   |
| `docker network prune`                  | Elimina las redes que no se usan.                           |
| `docker volume prune`                   | Elimina los volúmenes que no se usan.                       |

## Casos de prueba

| Carpeta                    | Contenido                                                                                                                                  |
| :------------------------- | :----------------------------------------------------------------------------------------------------------------------------------------- |
| `src/test/c/accept/`       | 20 programas que el compilador debe aceptar. Son programas completos y también válidos semánticamente, así que seguirán siéndolo en el Stage III. |
| `src/test/c/reject/`       | 5 programas que el compilador debe rechazar por errores léxicos o sintácticos.                                                             |
| `src/test/c/ignore/reject/`| 8 programas que deben rechazarse por errores semánticos. `test.sh` los lista pero no los ejecuta hasta que exista el análisis semántico.     |

## CI/CD

Para que cada _push_ o PR (_Pull Request_) dispare una integración automática, hay que activar _GitHub Actions_ en la pestaña _Settings_ del repositorio, con la siguiente configuración:

| Clave                                                      | Valor                                               |
| :--------------------------------------------------------- | :-------------------------------------------------- |
| `Actions permissions`                                      | `Allow all actions and reusable workflows`          |
| `Allow GitHub Actions to create and approve pull requests` | `false`                                             |
| `Artifact and log retention`                               | `30 days`                                           |
| `Fork pull request workflows from outside collaborators`   | `Require approval for all outside collaborators`    |
| `Workflow permissions`                                     | `Read repository contents and packages permissions` |

## Extensiones recomendadas

* [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
* [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)
* [Yash](https://marketplace.visualstudio.com/items?itemName=daohong-emilio.yash)
