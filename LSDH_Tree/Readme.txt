LSDh Tree

Link al dataset utilizado: https://drive.google.com/file/d/13sJYJJamjJvDKboH4U6o5e18mIPQHiSQ/view?usp=sharing

Para la construccion de esta estructura se necesita un template, el cual va a ser el tipo de dato con el que va a trabajar la estructura y 
como parametros esta va a recibir el numero de dimensiones que se utilizan y el numero de objetos maximos que puede almacenar un bucket antes
de hacer split.

En cuestion de insertado, el tipo de dato a recibir sera T*, siendo T el tipo de dato con el que va a trabajar la estructura. Para el insertado se puede 
hacer manualmente como esta en build_data_structure2(todas las refrencias son a main.cpp) o se puede hacer leyendo un archivo csv con la 
funcion read_record de input.h y recibiendo como parametro la estructura creada.

En cuestion de busqueda, esta la busqueda por knn como esta en query_knn y se recibe por parametro el numero k de vecinos a mostrar y un T* que vendria
a ser el objeto a buscar.

FALTA IMPLEMENTAR:
Esta estructura esta implementada casi a totalidad, la unica funcion faltante es la busqueda de regiones más cercanas al punto consulta
que utiliza la estructura para cuando no encuentra los suficientes elementos k en el bucket donde deberia estar el punto consulta y debe 
seguir desplazandose en el arbol.

Otra falta fuera de la estructura es un error al leer los datos del .csv en input.h

Para poder ejecutar esta estructura se necesitan los siguientes archivos:
-node.h
-spatial_object.h
-node_bucket.h
-node_directory.h
-distance_cal.h
-lsdh_tree.h
-input.h
-main.cpp

