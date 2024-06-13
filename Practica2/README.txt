ANALIZADOR LÉXICO DE LENGUAJE D CON FLEX - Luis Miguel Ares Seijas
GENERAR ANALIZADOR LÉXICO DE FLEX
flex --outfile=lexico.c flex.l

COMPILACIÓN
./compilar.sh (si no tiene permisos, ejecutar antes 'chmod +x compilar.sh')

EJECUCIÓN
./Compilador <archivo.d>

NOTA: Al compilar se arroja un warning de función no usada, pero forma parte del código generado por Flex, por lo que decidí no tocarlo.
