# Interfaces al usuario para el control de un Soft Gripper

Este repositorio contiene dos interfaces diseñadas para controlar un soft gripper, cada una con un enfoque distinto pero con el mismo objetivo: permitir el control de la apertura del gripper. Ambos archivos son funcionales, por lo que basta con utilizar uno de los dos según el tipo de control que se prefiera. Ambas versiones cuentan con un historial que permite al usuario ver el último movimiento generado.

El movimiento del gripper está controlado mediante tracción de cables, que son movidos por dos motores paso a paso **NEMA 17**. Este código se encarga de controlar los motores para generar el movimiento de apertura y cierre del gripper.

## Archivos

1. **botones.ino**: 
   - Esta versión usa tres botones para controlar la apertura del gripper en ambos lados de manera simultánea. Los botones permiten la acción de abrir o cerrar el gripper en ambas partes con un solo comando.

2. **deslizadores.ino**: 
   - En esta versión, se utilizan dos deslizadores que van de 0 a 100. Cada deslizador controla la apertura correspondiente de cada lado del gripper, donde 0 es el punto de partida (apertura) y 100 es una vuelta completa del motor (cierre máximo).

## Requisitos

- **Arduino IDE** para cargar el código.
- **Placa Arduino MKR Wifi 1010** o cualquier placa compatible con el código.
- **Motores NEMA 17** para controlar el movimiento de tracción de cables.
  
## Instrucciones de uso

1. Abre el código en Arduino IDE.
2. Conecta la placa Arduino MKR Wifi 1010 a tu computadora.
3. Selecciona la placa adecuada en el entorno de Arduino IDE.
4. Carga el código seleccionado en la placa.
5. Una vez cargado, se generará una dirección IP que podrás utilizar para acceder al control del gripper.
6. Abre un navegador web e ingresa la dirección IP generada para controlar el gripper de manera remota.

## Licencia

Este proyecto está bajo la Licencia MIT. Ver el archivo [LICENSE](LICENSE) para más detalles.

## Autora

- Tian Yao Lin
