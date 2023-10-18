#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<math.h>
#include<cstdlib>
#include<time.h>
#include<iostream>
#include<locale.h>
int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "");
	using namespace std;
	char esc;
	/*Esc servirá para while*/
	int num=0, cant=1, cont=0, bot=00001, top=99999999;
	/*Aquí se incluyen todas las variables que se utilizarán en la función random para generar un folio.*/
	float variable, iva, cdesc, disc;
	/*El valor del precio final*/
		printf("\n\t\t\t\t\t    INSTITUTO POLITÉCNICO NACIONAL\n");
		printf("\n\t\t\t\t    Centro de Estudios Científicos y Tecnológicos");
		printf("\n\t\t\t\t\t\t'Gonzalo Vázquez Vela'\n");
		printf("\n\t\t\t\t   Unidad de Aprendizaje: LENGUAJE DE PROGRAMACIÓN");
		printf("\n\n\t\t\t\t\t\tElaborado por: Equipo 1");
		printf("\n\t\t\t\t\t\t     Grupo: 4IV22");
		printf("\n\n");
		cout<<"\n Polimercado.";
		/*Nombre temporal del proyecto*/
		cout<<"\n\n Con base al siguiente menú, indique la sección de abarrotes que desee consultar.";
		cout<<"\n";
		/*Menú*/
		cout<<"\n a. Abarrotes";
		float aa=120,ab=50,ac=85.50,ad=111,ae=99,af=18.40,ag=14.50,ah=84.83,ai,aj=13.90,ak=55.50,al=62,am=45.62,an=36.10,ao=200,
		ap=46.10,aq=527,ar=8.25,as=45.50,at=40,au=23.60,av=18.70,aw=22,ax=5,ay=13,az=47.90,aaa=11.90,aab=35,aac=34.50,aad=82,aae=49.90,aaf=34.40,aag=29,aah=17.90;
		cout<<"\n b. Alimentos preparados";
		float ba=115,bb=42.90,bc=76,bd=132,be=64,bf=30,bg=69,bh=109;
		cout<<"\n c. Automedicación";
		float ca=18,cb=12.50,cc=54,cd=75,ce=24,cf=145,cg=102,ch=62.50;
		cout<<"\n d. Bebidas";
		float da=14.50,db=22.50,dc=13,dd=46,de=21,df=20,dg=12.50,dh=3.40,di=38,dj=55.60,dk=36,dl=39;
		cout<<"\n e. Bebidas alcohólicas";
		float ea=150,eb=284,ec=309,ed=463,ee=738,ef=585,eg=359,eh=222,ei=907,ej=20.90,ek=814,el=236;
		cout<<"\n f. Botanas";
		float fa=41,fb=54,fc=28.90,fd=32.90,fe=38,ff=35,fg=36;
		cout<<"\n g. Dulcería";
		float ga=13.50,gb=39.50,gc=38,gd=46.10,ge=26,gf=18,gg=41.50,gh=32.90,gi=49.90,gj=21.50,gk=44;
		cout<<"\n h. Frutas y verduras";
		float ha=62,hb=11,hc=13.90,hd=46.90,he=29.90,hf=36.90,hg=29.90,hh=49.90,hi=32.90,hj=26.90,hk=12.90,hl=32.90,
		hm=16.90,hn=59,ho=34.90;
		cout<<"\n i. Harina y pan";
		float ia=27,ib=12.40,ic=41,id=32.50,ie=29,ij=29.90,ig=30.30,ih=20.90,ii=14;
		cout<<"\n j. Higiene personal";
		float ja=41,jb=39.50,jc=36,jd=17,je=65,jf=179,jg=53.50,jh=57,ji=100,jj=105,jk=48.30,jl=130,jm=109,jn=79,jo=69,jp=259,jq=37.50,
		jr=290,js=145,jt=34,ju=35,jv=34.90,jw=219,jx=55.50,jy=113,jz=15,jaa=105,jab=41.90;
		cout<<"\n k. Lácteos";
		float ka=23.70,kb=17.50,kc=143,kd=16.90,ke=29.40,kf=61,kg=5.70,kh=22.50,ki=27.50,kj=29,kk=66,kl=49.90,km=9.35,kn=49;
		cout<<"\n l. Nevería";
		float la=30,lb=20,lc=50;
		cout<<"\n m. Productos de limpieza y jarcería";
		float ma=25,mb=23.50,mc=32.90,md=24,me=16.50,mf=21.90,mg=35,mh=55,mi=50,mj=27.50,mk=94,ml=55,mm=56,mn=28,mo=18.50,mp=32.50,mq=45,
		mr=22,ms=21.90,mt=26.50,mu=4.50,mv=22,mw=45.90;
		cout<<"\n n. Productos enlatados";
		float na=29,nb=34.90,nc=30,nd=22,ne=15.50,nf=52.50,ng=33.50,nh=11,ni=11,nj=20.50,nk=7.90,nl=13,nm=5.50,nn=29.90;
		cout<<"\n o. Uso doméstico";
		float oa=62,ob=18.50,oc=75,od=85,oe=310,of=199,og=36.51,oh=15,oi=57,oj=55,ok=11.50,ol=11.90,om=40.20,on=80,oo=18.90,op=19,oq=43.50,
		ox=85,os=70,ot=22.01,ou=55,ov=31.90,ow=25;
		do
		{
			char menu;
			/*Como la lista trabaja con base a letras, se utiliza char para guardar la variable*/
			cout<<"\n\n Sección que desea consultar: ";
			cin>> menu;
			cout<<"\n\n ";
			switch(menu)
			{
				case 'a':
					cout<<"a.a \t- Aceites comestibles \t\t\t -$120";
					cout<<"\n a.b \t- Aderezo \t\t\t\t -$50";
					cout<<"\n a.c \t- Consomé \t\t\t\t -$85.50";
					cout<<"\n a.d \t- Crema de cacahuate \t\t\t -$111";
					cout<<"\n a.e \t- Crema para café \t\t\t -$99";
					cout<<"\n a.f \t- Puré de tomate \t\t\t -$18.40";
					cout<<"\n a.g \t- Alimento para bebé \t\t\t -$14.50";
					cout<<"\n a.h \t- Alimento para mascotas \t\t -$84.83";
					cout<<"\n a.i \t- Atole \t\t\t\t -$8.50";
					cout<<"\n a.j \t- Avena \t\t\t\t -$13.90";
					cout<<"\n a.k \t- Azúcar \t\t\t\t -$55.50";
					cout<<"\n a.l \t- Café \t\t\t\t\t -$62";
					cout<<"\n a.m \t- Cereales \t\t\t\t -$45.62";
					cout<<"\n a.n \t- Chile piquín \t\t\t\t -$36.10";
					cout<<"\n a.o \t- Paquete de especias \t\t\t -$200";
					cout<<"\n a.p \t- Flan en polvo \t\t\t -$46.10";
					cout<<"\n a.q \t- Fórmulas infantiles \t\t\t -$527";
					cout<<"\n a.r \t- Gelatinas en polvo \t\t\t -$8.25";
					cout<<"\n a.s \t- Pimienta \t\t\t\t -$45.50";
					cout<<"\n a.t \t- Harina preparada \t\t\t -$40";
					cout<<"\n a.u \t- Mole \t\t\t\t\t -$23.60";
					cout<<"\n a.v \t- Sal \t\t\t\t\t -$18.70";
					cout<<"\n a.w \t- Salsas embotelladas \t\t\t -$22";
					cout<<"\n a.x \t- Sazonadores \t\t\t\t -$5";
					cout<<"\n a.y \t- Sopas de sobre \t\t\t -$13";
					cout<<"\n a.z \t- Cajeta \t\t\t\t -$47.90";
					cout<<"\n a.aa \t- Catsup \t\t\t\t -$11.90";
					cout<<"\n a.ab \t- Mayonesa \t\t\t\t -$35";
					cout<<"\n a.ac \t- Mermelada \t\t\t\t -$34.50";
					cout<<"\n a.ad \t- Miel \t\t\t\t\t -$82";
					cout<<"\n a.ae \t- Té \t\t\t\t\t -$49.90";
					cout<<"\n a.af \t- Vinagre \t\t\t\t -$34.40";
					cout<<"\n a.ag \t- Docena de huevos \t\t\t -$29";
					cout<<"\n a.ah \t- Pastas \t\t\t\t -$17.90";
				break;
				case 'b':
					cout<<"b.a \t- Pastas listas para comer \t\t -$115";
					cout<<"\n b.b \t- Salchicha \t\t\t\t -$42.90";
					cout<<"\n b.c \t- Mortadela \t\t\t\t -$76";
					cout<<"\n b.d \t- Tocino \t\t\t\t -$132";
					cout<<"\n b.e \t- Jamón \t\t\t\t -$64";
					cout<<"\n b.f \t- Manteca \t\t\t\t -$30";
					cout<<"\n b.g \t- Chorizo \t\t\t\t -$69";
					cout<<"\n b.h \t- Carne de puerco/res/pollo \t\t -$109";
				break;
				case 'c':
					cout<<"c.a \t- Suero \t\t\t\t -$18";
					cout<<"\n c.b \t- Agua oxigenada \t\t\t -$12.50";
					cout<<"\n c.c \t- Preservativos \t\t\t -$54";
					cout<<"\n c.d \t- Alcohol \t\t\t\t -$75";
					cout<<"\n c.e \t- Gasas \t\t\t\t -$24";
					cout<<"\n c.f \t- Analgésicos \t\t\t\t -$145";
					cout<<"\n c.g \t- Antigripales \t\t\t\t -$102";
					cout<<"\n c.h \t- Antiácidos \t\t\t\t -$62.50";
				break;
				case 'd':
					cout<<"d.a \t- Agua mineral \t\t\t\t -$14.50";
					cout<<"\n d.b \t- Agua natural \t\t\t\t -$22.50";
					cout<<"\n d.c \t- Agua saborizada \t\t\t -$13";
					cout<<"\n d.d \t- Jarabes \t\t\t\t -$46";
					cout<<"\n d.e \t- Jugos y néctares \t\t\t -$21";
					cout<<"\n d.f \t- Naranjadas \t\t\t\t -$20";
					cout<<"\n d.g \t- Bebidas de soya \t\t\t -$12.50";
					cout<<"\n d.h \t- Bebidas en polvo \t\t\t -$3.40";
					cout<<"\n d.i \t- Bebidas infantiles \t\t\t -$38";
					cout<<"\n d.j \t- Bebidas isotóncias \t\t\t -$55.60";
					cout<<"\n d.k \t- Energetizantes \t\t\t -$36";
					cout<<"\n d.l \t- Refrescos \t\t\t\t -$39";
				break;
				case 'e':
					cout<<"e.a \t- 6 Cervezas \t\t\t\t -$90";
					cout<<"\n e.b \t- Anís \t\t\t\t\t -$284";
					cout<<"\n e.c \t- Brandy \t\t\t\t -$309";
					cout<<"\n e.d \t- Ginebra \t\t\t\t -$463";
					cout<<"\n e.e \t- Cordiales \t\t\t\t -$738";
					cout<<"\n e.f \t- Mezcal \t\t\t\t -$585";
					cout<<"\n e.g \t- Jerez \t\t\t\t -$359";
					cout<<"\n e.h \t- Ron \t\t\t\t\t -$222";
					cout<<"\n e.i \t- Tequila \t\t\t\t -$907";
					cout<<"\n e.j \t- Sidra \t\t\t\t -$20.90";
					cout<<"\n e.k \t- Whiskey \t\t\t\t -$814";
					cout<<"\n e.l \t- Vodka \t\t\t\t -$236";
				break;
				case 'f':
					cout<<"f.a \t- Papas \t\t\t\t -$41";
					cout<<"\n f.b \t- Palomitas \t\t\t\t -$54";
					cout<<"\n f.c \t- Frituras de maíz \t\t\t -$28.90";
					cout<<"\n f.d \t- Cacahuates \t\t\t\t -$32.90";
					cout<<"\n f.e \t- Botanas saladas \t\t\t -$38";
					cout<<"\n f.f \t- Barras alimenticias \t\t\t -$35";
					cout<<"\n f.g \t- Nueces y semillas \t\t\t -$36";
				break;
				case 'g':
					cout<<"g.a \t- Caramelos \t\t\t\t -$13.50";
					cout<<"\n g.b \t- Dulces enchilados \t\t\t -$39.50";
					cout<<"\n g.c \t- Chocolate de mesa \t\t\t -$38";
					cout<<"\n g.d \t- Chocolate en polvo \t\t\t -$46.10";
					cout<<"\n g.e \t- Chocolates en barra \t\t\t -$26";
					cout<<"\n g.f \t- Chicles \t\t\t\t -$18";
					cout<<"\n g.g \t- Mazapán \t\t\t\t -$41.50";
					cout<<"\n g.h \t- Malvaviscos \t\t\t\t -$32.90";
					cout<<"\n g.i \t- Pulpa de tamarindo \t\t\t -$49.90";
					cout<<"\n g.j \t- Pastillas de dulce \t\t\t -$21.50";
					cout<<"\n g.k \t- Bolsa de paletas de dulce \t\t -$44";
				break;
				case 'h':
					cout<<"h.a \t- Aguacates \t\t\t\t -$62";
					cout<<"\n h.b \t- Cabeza de ajo \t\t\t -$11";
					cout<<"\n h.c \t- Cebollas \t\t\t\t -$13.90";
					cout<<"\n h.d \t- Chiles \t\t\t\t -$46.90";
					cout<<"\n h.e \t- Jitomates \t\t\t\t -$29.90";
					cout<<"\n h.f \t- Papas \t\t\t\t -$36.90";
					cout<<"\n h.g \t- Limones \t\t\t\t -$29.90";
					cout<<"\n h.h \t- Manzanas \t\t\t\t -$49.90";
					cout<<"\n h.i \t- Naranjas \t\t\t\t -$32.90";
					cout<<"\n h.j \t- Plátanos \t\t\t\t -$26.90";
					cout<<"\n h.k \t- Sandías \t\t\t\t -$12.90";
					cout<<"\n h.l \t- Mangos \t\t\t\t -$32.90";
					cout<<"\n h.m \t- Pepinos \t\t\t\t -$16.90";
					cout<<"\n h.n \t- Duraznos \t\t\t\t -$59";
					cout<<"\n h.o \t- Toronjas \t\t\t\t -$34.90";
				break;
				case 'i':
					cout<<"i.a \t- Tortillas de harina \t\t\t -$27";
					cout<<"\n i.b \t- Tortillas de maíz \t\t\t -$12.40";
					cout<<"\n i.c \t- Galletas dulces \t\t\t -$41";
					cout<<"\n i.d \t- Galletas saladas \t\t\t -$32.50";
					cout<<"\n i.e \t- Pastelillos \t\t\t\t -$29";
					cout<<"\n i.j \t- Pan de caja \t\t\t\t -$29.90";
					cout<<"\n i.g \t- Pan dulce \t\t\t\t -$30.30";
					cout<<"\n i.h \t- Pan molido \t\t\t\t -$20.90";
					cout<<"\n i.i \t- Pan tostado \t\t\t\t -$14";
				break;
				case 'j':
					cout<<"j.a \t- Totallas húmedas \t\t\t -$41";
					cout<<"\n j.b \t- Aceite para bebé \t\t\t -$39.50";
					cout<<"\n j.c \t- Toallas femeninas \t\t\t -$36";
					cout<<"\n j.d \t- Algodón \t\t\t\t -$17";
					cout<<"\n j.e \t- Tinte para el cabello \t\t -$65";
					cout<<"\n j.f \t- Biberones \t\t\t\t -$179";
					cout<<"\n j.g \t- Talco \t\t\t\t -$53.50";
					cout<<"\n j.h \t- Cepillo de dientes \t\t\t -$57";
					cout<<"\n j.i \t- Shampoo \t\t\t\t -$100";
					cout<<"\n j.j \t- Acondicionador \t\t\t -$105";
					cout<<"\n j.k \t- Cotonetes \t\t\t\t -$48.30";
					cout<<"\n j.l \t- Rastrillos \t\t\t\t -$130";
					cout<<"\n j.m \t- Crema facial \t\t\t\t -$109";
					cout<<"\n j.n \t- Crema corporal \t\t\t -$79";
					cout<<"\n j.o \t- Papel higiénico \t\t\t -$69";
					cout<<"\n j.p \t- Crema para afeitar \t\t\t -$259";
					cout<<"\n j.q \t- Pañuelos faciales \t\t\t -$37.50";
					cout<<"\n j.r \t- Papel higienico \t\t\t -$290";
					cout<<"\n j.s \t- Dentífricos \t\t\t\t -$145";
					cout<<"\n j.t \t- Pañuelos desechables \t\t\t -$34";
					cout<<"\n j.u \t- Desodorante en barra \t\t\t -$35";
					cout<<"\n j.v \t- Desodorante en aerosol \t\t -$34.90";
					cout<<"\n j.w \t- Maquillaje \t\t\t\t -$219";
					cout<<"\n j.x \t- Enjuague bucal \t\t\t -$55.50";
					cout<<"\n j.y \t- Lubricante para labios \t\t -$113";
					cout<<"\n j.z \t- Gel \t\t\t\t\t -$15";
					cout<<"\n j.aa \t- Loción hidratante \t\t\t -$105";
					cout<<"\n j.ab \t- Jabones corporales \t\t\t -$41.90";
				break;
				case 'k':
					cout<<"k.a \t- Leche condensada \t\t\t -$23.70";
					cout<<"\n k.b \t- Leche deslactosada \t\t\t -$17.50";
					cout<<"\n k.c \t- Leche en polvo \t\t\t -$143";
					cout<<"\n k.d \t- Leche evaporada \t\t\t -$16.90";
					cout<<"\n k.e \t- Leche light \t\t\t\t -$29.40";
					cout<<"\n k.f \t- Leche pasteurizada \t\t\t -$61";
					cout<<"\n k.g \t- Leche saborizada \t\t\t -$5.70";
					cout<<"\n k.h \t- Leche semidescremada \t\t\t -22.50$";
					cout<<"\n k.i \t- Crema \t\t\t\t -$27.50";
					cout<<"\n k.j \t- Yoghurt \t\t\t\t -$29";
					cout<<"\n k.k \t- Mantequilla \t\t\t\t -$66";
					cout<<"\n k.l \t- Margarina \t\t\t\t -$49.90";
					cout<<"\n k.m \t- Media crema \t\t\t\t -$9.35";
					cout<<"\n k.n \t- Queso \t\t\t\t -$49";
				break;
				case 'l':
					cout<<"l.a \t- Paletas de hielo \t\t\t -$30";
					cout<<"\n l.b \t- Sándwich de helado \t\t\t -$20";
					cout<<"\n l.c \t- Helado de un litro \t\t\t -$50";
				break;
				case 'm':
					cout<<"m.a \t- Velas \t\t\t\t -$25";
					cout<<"\n m.b \t- Cepillo de plástico \t\t\t -$23.50";
					cout<<"\n m.c \t- Vasos desechables \t\t\t -$32.90";
					cout<<"\n m.d \t- Cinta adhesiva \t\t\t -$24";
					cout<<"\n m.e \t- Cucharas de plástico \t\t\t -$16.50";
					cout<<"\n m.f \t- Escobas \t\t\t\t -$21.90";
					cout<<"\n m.g \t- Trapeadores \t\t\t\t -$35";
					cout<<"\n m.h \t- Mechudos \t\t\t\t -$55";
					cout<<"\n m.i \t- Trampas para rata \t\t\t -$50";
					cout<<"\n m.j \t- Tenedores de plástico \t\t -$27.50";
					cout<<"\n m.k \t- Extensiones \t\t\t\t -$94";
					cout<<"\n m.l \t- Multicontacto \t\t\t -$55";
					cout<<"\n m.m \t- Recogedor de metal \t\t\t -$56";
					cout<<"\n m.n \t- Recogedor de plástico \t\t -$28";
					cout<<"\n m.o \t- Popotes \t\t\t\t -$18.50";
					cout<<"\n m.p \t- Platos desechables \t\t\t -$32.50";
					cout<<"\n m.q \t- Focos \t\t\t\t -$45";
					cout<<"\n m.r \t- Fusibles \t\t\t\t -$22";
					cout<<"\n m.s \t- Jergas \t\t\t\t -$21.90";
					cout<<"\n m.t \t- Franelas \t\t\t\t -$26.50";
					cout<<"\n m.u \t- Matamoscas \t\t\t\t -$4.50";
					cout<<"\n m.v \t- Pegamento \t\t\t\t -$22";
					cout<<"\n m.w \t- Mecate \t\t\t\t -$45.90";
				break;
				case 'n':
					cout<<"n.a \t- Aceitunas \t\t\t\t -$29";
					cout<<"\n n.b \t- Champiñones \t\t\t\t -$34.90";
					cout<<"\n n.c \t- Chícharo con zanahoria \t\t -$30";
					cout<<"\n n.d \t- Chícharos enlatados \t\t\t -$22";
					cout<<"\n n.e \t- Frijoles enlatados \t\t\t -$15.50";
					cout<<"\n n.f \t- Frutas en almíbar \t\t\t -$52.50";
					cout<<"\n n.g \t- Sardinas \t\t\t\t -$33.50";
					cout<<"\n n.h \t- Atún en agua \t\t\t\t -$11";
					cout<<"\n n.i \t- Atún en aceite \t\t\t -$11";
					cout<<"\n n.j \t- Chiles enlatados \t\t\t -$20.50";
					cout<<"\n n.k \t- Ensalada enlatada \t\t\t -$7.90";
					cout<<"\n n.l \t- Granos de elote enlatados \t\t -$13";
					cout<<"\n n.m \t- Sopa en lata \t\t\t\t -$5.50";
					cout<<"\n n.n \t- Vegetales en conversa \t\t -$29.90";
				break;
				case 'o':
					cout<<"o.a \t- Suavizante de telas \t\t\t -$62";
					cout<<"\n o.b \t- Ácido muriático \t\t\t -$18.50";
					cout<<"\n o.c \t- Sosa caustica \t\t\t -$75";
					cout<<"\n o.d \t- Aluminio \t\t\t\t -$85";
					cout<<"\n o.e \t- Pilas \t\t\t\t -$310";
					cout<<"\n o.f \t- Jabón liquido \t\t\t -$199";
					cout<<"\n o.g \t- Servilletas \t\t\t\t -$36.51";
					cout<<"\n o.h \t- Servitoallas \t\t\t\t -$15";
					cout<<"\n o.i \t- Aromatizante \t\t\t\t -$57";
					cout<<"\n o.j \t- Cera para automóvil \t\t\t -$55";
					cout<<"\n o.k \t- Cera para calzado \t\t\t -$11.50";
					cout<<"\n o.l \t- Pastillas sanitarias \t\t\t -$11.90";
					cout<<"\n o.m \t- Limpiadores liquidos \t\t\t -$40.20";
					cout<<"\n o.n \t- Limpiadores para pisos \t\t -$80";
					cout<<"\n o.o \t- Jabón de barra \t\t\t -$18.90";
					cout<<"\n o.p \t- Cerillos \t\t\t\t -$19";
					cout<<"\n o.q \t- Cloro \t\t\t\t -$43.50";
					cout<<"\n o.x \t- Blanqueador \t\t\t\t -$85";
					cout<<"\n o.s \t- Insecticidas \t\t\t\t -$70";
					cout<<"\n o.t \t- Fibras limpiadoras \t\t\t -$22.01";
					cout<<"\n o.u \t- Desinfectantes \t\t\t -$55";
					cout<<"\n o.v \t- Detergentes para trastes \t\t -$31.90";
					cout<<"\n o.w \t- Detergente para ropa \t\t\t -$25";
				break;
				default:
					cout<<"No pertenece a una opción indicada en el menú.";
				break;	
			}
			cout<<"\n\n Para seleccionar otra categoría del menú, indíquelo mediante la letra 'Z'. De lo contrario, presione otra tecla\n para continuar con la compra: ";
			cin>> esc;
		}
		while(esc=='Z');
		/*Se ejecutará la opción del menú y la sentencia switch hasta que se deje de ingresar Z en la opción anterior*/
		string producto="";
		int compra=0;
		float n, precio;
		/*El problema es que se tendría que guardar x como char x[3] toda vez que lo que buscamos que indique el usuario en "Ingrese el 'n' producto que se va a comprar" es la variable con que se enlista el producto (aa, ab, ca... etc.); sin embargo,el programa no guarda y reconoce dichos caracteres como aa=120 y así, tal y como se definieron al principio en un float. Es decir, en lugar de reconocer al primer (ej. aa) y segundo (ej. ab) producto como valores numéricos (120 y 50) para efectuar 120+50=$170 lo ve como aa+ab=No se puede*/
		cout<<"\n\n Introduzca cuántos productos va a comprar: ";
		cin>>compra;
		n=1;
		precio=0;
		do
		{
			printf("\n Ingrese %2.0f° producto con base al menú anterior: ", n);
			cin>>producto;
			/*Se asigna o define un valor para cada producto y se identifica con otra variable, puesto que si no se introduce un número, no se ejecturá correctamente el loop.*/
			if(producto=="a.a")
			{
				precio=precio+(aa);
			}
			else if(producto=="a.b")
			{
				precio=precio+(ab);
			}
			if(producto=="a.c")
			{
				precio=precio+(ac);
			}
			else if(producto=="a.d")
			{
				precio=precio+(ad);
			}
			if(producto=="a.e")
			{
				precio=precio+(ae);
			}
			else if(producto=="a.f")
			{
				precio=precio+(af);
			}
			if(producto=="a.g")
			{
				precio=precio+(ag);
			}
			else if(producto=="a.h")
			{
				precio=precio+(ah);
			}
			if(producto=="a.i")
			{
				precio=precio+(ai);
			}
			else if(producto=="a.j")
			{
				precio=precio+(aj);
			}
			if(producto=="a.k")
			{
				precio=precio+(ak);
			}
			else if(producto=="a.l")
			{
				precio=precio+(al);
			}
			if(producto=="a.m")
			{
				precio=precio+(am);
			}
			else if(producto=="a.n")
			{
				precio=precio+(an);
			}
			if(producto=="a.o")
			{
				precio=precio+(ao);
			}
			else if(producto=="a.p")
			{
				precio=precio+(ap);
			}
			if(producto=="a.q")
			{
				precio=precio+(aq);
			}
			else if(producto=="a.r")
			{
				precio=precio+(ar);
			}
			if(producto=="a.s")
			{
				precio=precio+(as);
			}
			else if(producto=="a.t")
			{
				precio=precio+(at);
			}
			if(producto=="a.u")
			{
				precio=precio+(au);
			}
			else if(producto=="a.v")
			{
				precio=precio+(av);
			}
			if(producto=="a.w")
			{
				precio=precio+(aw);
			}
			else if(producto=="a.x")
			{
				precio=precio+(ax);
			}
			if(producto=="a.y")
			{
				precio=precio+(ay);
			}
			else if(producto=="a.z")
			{
				precio=precio+(az);
			}
			if(producto=="a.aa")
			{
				precio=precio+(aaa);
			}
			else if(producto=="a.ab")
			{
				precio=precio+(aab);
			}
			if(producto=="a.ac")
			{
				precio=precio+(aac);
			}
			else if(producto=="a.ad")
			{
				precio=precio+(aad);
			}
			if(producto=="a.ae")
			{
				precio=precio+(aae);
			}
			else if(producto=="a.af")
			{
				precio=precio+(aaf);
			}
			if(producto=="a.ag")
			{
				precio=precio+(aag);
			}
			else if(producto=="a.ah")
			{
				precio=precio+(aah);
			}
			
			if(producto=="b.a")
			{
				precio=precio+(ba);
			}
			else if(producto=="b.b")
			{
				precio=precio+(bb);
			}
			if(producto=="b.c")
			{
				precio=precio+(bc);
			}
			else if(producto=="b.d")
			{
				precio=precio+(bd);
			}
			if(producto=="b.e")
			{
				precio=precio+(be);
			}
			else if(producto=="b.f")
			{
				precio=precio+(bf);
			}
			if(producto=="b.g")
			{
				precio=precio+(bg);
			}
			else if(producto=="b.h")
			{
				precio=precio+(bh);
			}
			
			if(producto=="c.a")
			{
				precio=precio+(ca);
			}
			else if(producto=="c.b")
			{
				precio=precio+(cb);
			}
			if(producto=="c.c")
			{
				precio=precio+(cc);
			}
			else if(producto=="c.d")
			{
				precio=precio+(cd);
			}
			if(producto=="c.e")
			{
				precio=precio+(ce);
			}
			else if(producto=="c.f")
			{
				precio=precio+(cf);
			}
			if(producto=="c.g")
			{
				precio=precio+(cg);
			}
			else if(producto=="c.h")
			{
				precio=precio+(ch);
			}
			
			if(producto=="d.a")
			{
				precio=precio+(da);
			}
			else if(producto=="d.b")
			{
				precio=precio+(db);
			}
			if(producto=="d.c")
			{
				precio=precio+(dc);
			}
			else if(producto=="d.d")
			{
				precio=precio+(dd);
			}
			if(producto=="d.e")
			{
				precio=precio+(de);
			}
			else if(producto=="d.f")
			{
				precio=precio+(df);
			}
			if(producto=="d.g")
			{
				precio=precio+(dg);
			}
			else if(producto=="d.h")
			{
				precio=precio+(dh);
			}
			if(producto=="d.i")
			{
				precio=precio+(di);
			}
			else if(producto=="d.j")
			{
				precio=precio+(dj);
			}
			if(producto=="d.k")
			{
				precio=precio+(dk);
			}
			else if(producto=="d.l")
			{
				precio=precio+(dl);
			}
			
			if(producto=="e.a")
			{
				precio=precio+(ea);
			}
			else if(producto=="e.b")
			{
				precio=precio+(eb);
			}
			if(producto=="e.c")
			{
				precio=precio+(ec);
			}
			else if(producto=="e.d")
			{
				precio=precio+(ed);
			}
			if(producto=="e.e")
			{
				precio=precio+(ee);
			}
			else if(producto=="e.f")
			{
				precio=precio+(ef);
			}
			if(producto=="e.g")
			{
				precio=precio+(eg);
			}
			else if(producto=="e.h")
			{
				precio=precio+(eh);
			}
			if(producto=="e.i")
			{
				precio=precio+(ei);
			}
			else if(producto=="e.j")
			{
				precio=precio+(ej);
			}
			if(producto=="e.k")
			{
				precio=precio+(ek);
			}
			else if(producto=="e.l")
			{
				precio=precio+(el);
			}
			
			if(producto=="f.a")
			{
				precio=precio+(fa);
			}
			else if(producto=="f.b")
			{
				precio=precio+(fb);
			}
			if(producto=="f.c")
			{
				precio=precio+(fc);
			}
			else if(producto=="f.d")
			{
				precio=precio+(fd);
			}
			if(producto=="f.e")
			{
				precio=precio+(fe);
			}
			else if(producto=="f.f")
			{
				precio=precio+(ff);
			}
			if(producto=="f.g")
			{
				precio=precio+(fg);
			}
			
			else if(producto=="g.a")
			{
				precio=precio+(ga);
			}
			if(producto=="g.b")
			{
				precio=precio+(gb);
			}
			else if(producto=="g.c")
			{
				precio=precio+(gc);
			}
			if(producto=="g.d")
			{
				precio=precio+(gd);
			}
			else if(producto=="g.e")
			{
				precio=precio+(ge);
			}
			if(producto=="g.f")
			{
				precio=precio+(gf);
			}
			else if(producto=="g.g")
			{
				precio=precio+(gg);
			}
			if(producto=="g.h")
			{
				precio=precio+(gh);
			}
			else if(producto=="g.i")
			{
				precio=precio+(gi);
			}
			if(producto=="g.j")
			{
				precio=precio+(gj);
			}
			else if(producto=="g.k")
			{
				precio=precio+(gk);
			}
			
			if(producto=="h.a")
			{
				precio=precio+(ha);
			}
			else if(producto=="h.b")
			{
				precio=precio+(hb);
			}
			if(producto=="h.c")
			{
				precio=precio+(hc);
			}
			else if(producto=="h.d")
			{
				precio=precio+(hd);
			}
			if(producto=="h.e")
			{
				precio=precio+(he);
			}
			else if(producto=="h.f")
			{
				precio=precio+(hf);
			}
			if(producto=="h.g")
			{
				precio=precio+(hg);
			}
			else if(producto=="h.h")
			{
				precio=precio+(hh);
			}
			if(producto=="h.i")
			{
				precio=precio+(hi);
			}
			else if(producto=="h.j")
			{
				precio=precio+(hj);
			}
			if(producto=="h.k")
			{
				precio=precio+(hk);
			}
			else if(producto=="h.l")
			{
				precio=precio+(hl);
			}
			if(producto=="h.m")
			{
				precio=precio+(hm);
			}
			else if(producto=="h.n")
			{
				precio=precio+(hn);
			}
			if(producto=="h.o")
			{
				precio=precio+(ho);
			}
			
			else if(producto=="i.a")
			{
				precio=precio+(ia);
			}
			if(producto=="i.b")
			{
				precio=precio+(ib);
			}
			else if(producto=="i.c")
			{
				precio=precio+(ic);
			}
			if(producto=="i.d")
			{
				precio=precio+(id);
			}
			else if(producto=="i.e")
			{
				precio=precio+(ie);
			}
			if(producto=="i.j")
			{
				precio=precio+(ij);
			}
			else if(producto=="i.g")
			{
				precio=precio+(ig);
			}
			if(producto=="i.h")
			{
				precio=precio+(ih);
			}
			else if(producto=="i.i")
			{
				precio=precio+(ii);
			}
			
			if(producto=="j.a")
			{
				precio=precio+(ja);
			}
			else if(producto=="j.b")
			{
				precio=precio+(jb);
			}
			if(producto=="j.c")
			{
				precio=precio+(jc);
			}
			else if(producto=="j.d")
			{
				precio=precio+(jd);
			}
			if(producto=="j.e")
			{
				precio=precio+(je);
			}
			else if(producto=="j.f")
			{
				precio=precio+(jf);
			}
			if(producto=="j.g")
			{
				precio=precio+(jg);
			}
			else if(producto=="j.h")
			{
				precio=precio+(jh);
			}
			if(producto=="j.i")
			{
				precio=precio+(ji);
			}
			else if(producto=="j.j")
			{
				precio=precio+(jj);
			}
			if(producto=="j.k")
			{
				precio=precio+(jk);
			}
			else if(producto=="j.l")
			{
				precio=precio+(jl);
			}
			if(producto=="j.m")
			{
				precio=precio+(jm);
			}
			else if(producto=="j.n")
			{
				precio=precio+(jn);
			}
			if(producto=="j.o")
			{
				precio=precio+(jo);
			}
			else if(producto=="j.p")
			{
				precio=precio+(jp);
			}
			if(producto=="j.q")
			{
				precio=precio+(jq);
			}
			else if(producto=="j.r")
			{
				precio=precio+(jr);
			}
			if(producto=="j.s")
			{
				precio=precio+(js);
			}
			else if(producto=="j.t")
			{
				precio=precio+(jt);
			}
			if(producto=="j.u")
			{
				precio=precio+(ju);
			}
			else if(producto=="j.v")
			{
				precio=precio+(jv);
			}
			if(producto=="j.w")
			{
				precio=precio+(jw);
			}
			else if(producto=="j.x")
			{
				precio=precio+(jx);
			}
			if(producto=="j.y")
			{
				precio=precio+(jy);
			}
			else if(producto=="j.z")
			{
				precio=precio+(jz);
			}
			if(producto=="j.aa")
			{
				precio=precio+(jaa);
			}
			else if(producto=="j.ab")
			{
				precio=precio+(jab);
			}
			
			if(producto=="k.a")
			{
				precio=precio+(ka);
			}
			else if(producto=="k.b")
			{
				precio=precio+(kb);
			}
			if(producto=="k.c")
			{
				precio=precio+(kc);
			}
			else if(producto=="k.d")
			{
				precio=precio+(kd);
			}
			if(producto=="k.e")
			{
				precio=precio+(ke);
			}
			else if(producto=="k.f")
			{
				precio=precio+(kf);
			}
			if(producto=="k.g")
			{
				precio=precio+(kg);
			}
			else if(producto=="k.h")
			{
				precio=precio+(kh);
			}
			if(producto=="k.i")
			{
				precio=precio+(ki);
			}
			else if(producto=="k.j")
			{
				precio=precio+(kj);
			}
			if(producto=="k.k")
			{
				precio=precio+(kk);
			}
			else if(producto=="k.l")
			{
				precio=precio+(kl);
			}
			if(producto=="k.m")
			{
				precio=precio+(km);
			}
			else if(producto=="k.n")
			{
				precio=precio+(kn);
			}
			
			if(producto=="l.a")
			{
				precio=precio+(la);
			}
			else if(producto=="l.b")
			{
				precio=precio+(lb);
			}
			if(producto=="l.c")
			{
				precio=precio+(lc);
			}
			
			else if(producto=="m.a")
			{
				precio=precio+(ma);
			}
			if(producto=="m.b")
			{
				precio=precio+(mb);
			}
			else if(producto=="m.c")
			{
				precio=precio+(mc);
			}
			if(producto=="m.d")
			{
				precio=precio+(md);
			}
			else if(producto=="m.e")
			{
				precio=precio+(me);
			}
			if(producto=="m.f")
			{
				precio=precio+(mf);
			}
			else if(producto=="m.g")
			{
				precio=precio+(mg);
			}
			if(producto=="m.h")
			{
				precio=precio+(mh);
			}
			else if(producto=="m.i")
			{
				precio=precio+(mi);
			}
			if(producto=="m.j")
			{
				precio=precio+(mj);
			}
			else if(producto=="m.k")
			{
				precio=precio+(mk);
			}
			if(producto=="m.l")
			{
				precio=precio+(ml);
			}
			else if(producto=="m.m")
			{
				precio=precio+(mm);
			}
			if(producto=="m.n")
			{
				precio=precio+(mn);
			}
			else if(producto=="m.o")
			{
				precio=precio+(mo);
			}
			if(producto=="m.p")
			{
				precio=precio+(mp);
			}
			else if(producto=="m.q")
			{
				precio=precio+(mq);
			}
			if(producto=="m.r")
			{
				precio=precio+(mr);
			}
			else if(producto=="m.s")
			{
				precio=precio+(ms);
			}
			if(producto=="m.t")
			{
				precio=precio+(mt);
			}
			else if(producto=="m.u")
			{
				precio=precio+(mu);
			}
			if(producto=="m.v")
			{
				precio=precio+(mv);
			}
			else if(producto=="m.w")
			{
				precio=precio+(mw);
			}
			
			if(producto=="n.a")
			{
				precio=precio+(na);
			}
			else if(producto=="n.b")
			{
				precio=precio+(nb);
			}
			if(producto=="n.c")
			{
				precio=precio+(nc);
			}
			else if(producto=="n.d")
			{
				precio=precio+(nd);
			}
			if(producto=="n.e")
			{
				precio=precio+(ne);
			}
			else if(producto=="n.f")
			{
				precio=precio+(nf);
			}
			if(producto=="n.g")
			{
				precio=precio+(ng);
			}
			else if(producto=="n.h")
			{
				precio=precio+(nh);
			}
			if(producto=="n.i")
			{
				precio=precio+(ni);
			}
			else if(producto=="n.j")
			{
				precio=precio+(nj);
			}
			if(producto=="n.k")
			{
				precio=precio+(nk);
			}
			else if(producto=="n.l")
			{
				precio=precio+(nl);
			}
			if(producto=="n.m")
			{
				precio=precio+(nm);
			}
			else if(producto=="n.n")
			{
				precio=precio+(nn);
			}
			
			if(producto=="o.a")
			{
				precio=precio+(oa);
			}
			else if(producto=="o.b")
			{
				precio=precio+(ob);
			}
			if(producto=="o.c")
			{
				precio=precio+(oc);
			}
			else if(producto=="o.d")
			{
				precio=precio+(od);
			}
			if(producto=="o.e")
			{
				precio=precio+(oe);
			}
			else if(producto=="o.f")
			{
				precio=precio+(of);
			}
			if(producto=="o.g")
			{
				precio=precio+(og);
			}
			else if(producto=="o.h")
			{
				precio=precio+(oh);
			}
			if(producto=="o.i")
			{
				precio=precio+(oi);
			}
			else if(producto=="o.j")
			{
				precio=precio+(oj);
			}
			if(producto=="o.k")
			{
				precio=precio+(ok);
			}
			else if(producto=="o.l")
			{
				precio=precio+(ol);
			}
			if(producto=="o.m")
			{
				precio=precio+(om);
			}
			else if(producto=="o.n")
			{
				precio=precio+(on);
			}
			if(producto=="o.o")
			{
				precio=precio+(oo);
			}
			else if(producto=="o.p")
			{
				precio=precio+(op);
			}
			if(producto=="o.q")
			{
				precio=precio+(oq);
			}
			else if(producto=="o.x")
			{
				precio=precio+(ox);
			}
			if(producto=="o.s")
			{
				precio=precio+(os);
			}
			else if(producto=="o.t")
			{
				precio=precio+(ot);
			}
			if(producto=="o.u")
			{
				precio=precio+(ou);
			}
			else if(producto=="o.v")
			{
				precio=precio+(ov);
			}
			if(producto=="o.w")
			{
				precio=precio+(ow);
			}
			n++;
		}
		while(n<=compra);
		/*x guarda las VARIABLES con que se RECONOCEN a los productos*/
		iva=precio*0.16;
		variable=precio+iva;
		disc=variable*0.1;
		cdesc=variable-disc;
		if(variable>=0 && variable<1250)
		{
			cout<<"\n\n El total a pagar es $"<<variable<<" con IVA incluido del 16%"<<endl;
		}
		else
		{
			cout<<"\n\n El total a pagar es $"<<cdesc<<" con descuento incluido del 10% e IVA del 16%"<<endl;
		}
		char correo;
		cout<<"\n\n Ingrese su correo electrónico: ";
		cin>>correo;
		cout<<"\n El sistema agendará una cita que será enviada a su correo.";
		srand(time(NULL));
		while(cont<cant)
		/*Generará una cantidad de cinco dígitos aleatoriamente que servirá como folio.*/
		{
			num=bot+rand()%((top+1)-bot);
			/*La cantidad irá desde el 00001 hasta el 99999 de manera aleatoria*/
			cout<<"\n\n Utilice el folio que se muestra a continuación para recoger su orden: "<<num;
			cont++;
		}
	getch();
	return 0;
}
