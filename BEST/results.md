double f(double x){ return 13.056*x*x*x*x*x + 23.56f*x*x + 51.41*x; }
fitness = Suma(errorcuadratico)/numero de muestras = error promedio en la funcion

const int max_g = 50;
const int pop_size = 10000;
const int max_depth = 6 ;

const double m_rate = 0.9f;//0.1f
const double c_rate = 0.9f;
const double cross_prop = 0.1f;
    
const int fill_opt = HALF_FILL;
const int m_opt = SUBT_MUT;//oper_mut       
const int c_opt = PROP_CROSS;
const int s_opt = TOURN;

const int VAR_SIZ =2;
const int samples =1000000;

enum nodeTypes{_unop,_binop,_var,_term};                                                    
double** _values;
//std::basic_string<char> terRep[] = {"1","2","3","4","5","6","7","8","9"};
std::basic_string<char> terRep[] = {"1","2","3","4","5","6","7","8","9","sqt(2)","sqt(3)","pi","e"};
std::basic_string<char> binOpRep[] = {"+","-","*","/"};
//std::basic_string<char> binOpRep[] = {"+","-","*","/","^"};
std::basic_string<char> unOpRep[]  = {"exp","ln","sin","cos"};
std::basic_string<char> varRep[] = { "x","y"};//the last element is the F(x)    

#############################################################################
mejor individuo de la generacion 0
fitness: 76427400.098738
( / ( * ( * ( - ( * ( - e x )( * x x ) )( - ( * pi x )( * x x ) ) )( + ( - ( * x x )( - x 4 ) )( + ( * x x )( - x x ) ) ) )( / ( / ( / ( * x x )( - x x ) )( + ( / x sqt(3) )( / x x ) ) )( - ( * ( - 2 x )( + x x ) )( * ( * x 3 )( - x x ) ) ) ) )( * x x ) )

mejor individuo de la generacion 18
fitness: 0.080405
( * ( - ( * ( + ( / ( / 4 x )( + x x ) )( + ( / x sqt(2) )( - e 3 ) ) )x )( - ( + ( / ( / x 9 )( - x x ) )x )( / ( / x sqt(3) )( / 3 x ) ) ) )( * ( - ( + ( / ( * x x )( * x x ) )9 )( * ( / x x )( / x 2 ) ) )( - x ( / ( + ( + x 1 )( - x x ) )x ) ) ) )
#############################################################################
mejor individuo de la generacion 0
fitness: 9737916.437073
( + ( - ( + ( / ( * ( * x sqt(3) )( - x 3 ) )( * ( - x x )( / x x ) ) )( * ( + ( - x x )( - x 9 ) )( + ( / x x )( + x x ) ) ) )( / ( * ( * ( + sqt(3) x )( * 5 8 ) )( + ( + x x )( - x x ) ) )( * ( / ( * x x )( / x x ) )( * ( * x 9 )( + x x ) ) ) ) )5 )

mejor individuo de la generacion 21
fitness: 0.529613
( * ( - ( * ( - ( / ( / x x )pi )( - ( - x x )( + x x ) ) )( + ( - ( / 2 7 )( + x x ) )( * ( * x x )( * 6 x ) ) ) )( - ( - ( * x x )( / 8 x ) )( * ( / ( * x x )( / x x ) )( + ( * x x )x ) ) ) )x )
#############################################################################
mejor individuo de la generacion 0
fitness: 1081386.111321
( * ( + ( + ( - ( / ( / x x )( - x x ) )( * ( - x x )( + x 2 ) ) )( - ( + ( - x x )( * x x ) )( + ( - 6 x )( + x x ) ) ) )( - ( + ( + ( + sqt(3) x )( * x x ) )( / ( * x x )( + x 7 ) ) )( - ( * ( / x x )( + x x ) )( + ( * x x )( * x 5 ) ) ) ) )( * 9 ( / 9 ( / ( / x x )x ) ) ) )

mejor individuo de la generacion 12
fitness: 5.579119
( / ( * ( * ( + ( / ( / x x )x )( + ( + x x )( * x x ) ) )( + ( * ( * x x )( / e x ) )( * ( / x x )( - x x ) ) ) )( - ( / ( / ( + x x )( + x x ) )( * ( - x x )x ) )( - ( + ( / x x )( + x x ) )( / ( * pi 1 )x ) ) ) )( - ( * x sqt(3) )( - x x ) ) )
##############################################################################
mejor individuo de la generacion 0
fitness: 12682226.324863
( * ( - ( + ( + ( / ( + x x )( * 9 x ) )( / ( / x x )( / x sqt(3) ) ) )( * ( * ( + x x )( - x pi ) )( + ( / x x )( * x x ) ) ) )( + ( - ( * ( * x 8 )( * sqt(2) x ) )( - ( * x x )( / 4 x ) ) )( - ( * ( - pi 3 )( / x x ) )( + ( * 1 x )( - x x ) ) ) ) )( * x 7 ) )

mejor individuo de la generacion 25
fitness: 7.333046
( * ( - ( + ( - ( / ( - x 6 )( + x x ) )( * ( * x x )sqt(2) ) )( * ( * ( + x x )( - x sqt(2) ) )( + ( + x 6 )( + sqt(3) x ) ) ) )( * ( + x x )4 ) )( * ( * x x )pi ) )
###############################################################################
mejor de la generacion 0
fitness: 14196862.756203
( - ( * ( - ( + ( - ( / x x )( - 4 x ) )( - ( * x x )( * 7 x ) ) )( / ( * ( - x x )( * x x ) )( - ( * sqt(2) 9 )( - 8 x ) ) ) )( * ( - ( + ( + x x )( - 7 5 ) )( + ( / x 7 )( - x sqt(3) ) ) )( * ( * ( * x x )( * 4 x ) )( + ( / 4 x )( - x x ) ) ) ) )6 )

mejr de la generacion 19
fitness: 84.767060
( * ( * ( + ( * ( * x x )sqt(2) )( - ( + ( + x x )( + 2 x ) )( / ( + 6 pi )( - x x ) ) ) )( * ( - ( / ( - 4 sqt(3) )x )( + ( - x e )( - x x ) ) )( / ( / ( - x 3 )( + x 5 ) )( - ( - sqt(2) sqt(3) )( - 9 x ) ) ) ) )( * 9 x ) )
##############################################################################
