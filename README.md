# Interfaces al usuario para el control de un Soft Gripper

Este repositorio contiene dos interfaces diseñadas para controlar un soft gripper, cada una con un enfoque distinto pero con el mismo objetivo: permitir el control de la apertura del gripper. Ambos archivos son funcionales, por lo que basta con utilizar uno de los dos según el tipo de control que se prefiera.

## Archivos

1. **botones.ino**: 
   - Esta versión usa tres botones para controlar la apertura del gripper en ambos lados de manera simultánea. Los botones permiten la acción de abrir o cerrar el gripper en ambas partes con un solo comando.
   - **Historial**: Esta interfaz registra y muestra el último movimiento realizado, facilitando la referencia del estado actual del gripper.

2. **deslizadores.ino**: 
   - En esta versión, se utilizan dos deslizadores que van de 0 a 100. Cada deslizador controla la apertura correspondiente de cada lado del gripper, donde 0 es el punto de partida (sin apertura) y 100 es una vuelta completa (apertura máxima).
   - **Historial**: Al igual que la versión de botones, esta interfaz también guarda el último movimiento realizado y lo muestra al usuario para seguimiento.

## Requisitos

- **Arduino IDE** para cargar el código.
- **Placa Arduino MKR Wifi 1010** o cualquier placa compatible con el código.
  
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
