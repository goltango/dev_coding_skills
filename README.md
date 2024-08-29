# dev_coding_skills
A quick test to meassure skills

Tiempo de resolución estimado: 3 horas
Desarrolle una función en C en base a los siguientes requerimientos

La función deberá procesar transacciones de combustible enviadas por diferentes surtidores a través de una interfaz de comunicación serial. Dichas transacciones se almacenan en un buffer en forma contigua sin bytes de padding y con el endianess utilizado por el sistema. La Figura 1 muestra la estructura de los datos recibidos. En la tabla 1 están detallados los significados de los campos que forman una transacción.

![alt text](https://github.com/goltango/dev_coding_skills/blob/main/image1.png?raw=true)

Cada cierto tiempo, cuando se acumulan una cierta de cantidad de transacciones o transcurre un determinado tiempo la función será llamada teniendo que generar un log de los datos recibidos en otro buffer. El formato del log deberá respetar las siguientes cuestiones
* Cada transacción deberá ser reportada en ASCII con el siguiente formato [dd/mm/yy hh:mm:ss] id: DDDDD, reg: AAA 1111, prod: A, ltrs: ±DDDDDDD\n
* Las transacciones deberán ser ordenadas por fecha de menos recientes a más recientes ya que pueden encontrarse desordenadas en la memoria.

Así mismo, deberá responder al prototipo que se muestra más abajo. Es decir, recibirá un puntero al inicio del buffer que contiene las transacciones recibidas, el número de transacciones en el buffer y un puntero al inicio del buffer donde deberá escribir el log generado. El valor devuelto debe indicar la cantidad total de bytes escritos. Considerar que tendría que ser capaz de procesar hasta 100 transacciones.

int log_transactions(const char ∗data, char ∗log, size_t transaction_count);

La solución propuesta deberá minimizar el uso de memoria en el stack, evitar el acceso a datos desalineados de la memoria para mayor portabilidad. La plataforma de desarrollo esta especificada en la Tabla 2. Es posible, en caso que lo considere necesario, factorizar la función requerida en varias funciones. Se presenta el siguiente ejemplo de una salida valida que debería encontrarse en la región de memoria a la que apunta el puntero log una vez ejecutada la función solicitada:

[05/02/22 23:16:27] id: 01581, reg: AXJ 3658, prod: G, ltrs: +0000080\n
[05/02/22 23:45:33] id: 01303, reg: YCC 9001, prod: G, ltrs: +0000323\n
[06/02/22 00:16:03] id: 03021, reg: XTT 5547, prod: A, ltrs: -0010000\n

Se espera:
* Una respuesta testeada.
* Un buen manejo del lenguaje y el uso de buenas practicas.
* Una buena presentación.
Se valorará:
* Soluciones optimizadas que demuestren entendimiento de cuestiones relacionadas al desarrollo de sistema de embebidos.
* Comentarios que documenten con buen nivel de detalle.

Subir la solución propuesta junto con los recursos utilizados para su desarrollo a un repositorio en GitHub y enviar el link.
