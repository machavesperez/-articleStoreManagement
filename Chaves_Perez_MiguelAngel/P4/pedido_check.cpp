/*
 * $Id:$
 * Programa que hace algunas comprobaciones en el código fuente de
 * la práctica 3 de Programación Orientada a Objetos
 * POO - P3
 * © 2014-16 Pedro Delgado
 */
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchersInternal.h"
#include "clang/ASTMatchers/ASTMatchersMacros.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/SourceManager.h"
 
#include "llvm/Support/CommandLine.h"
#include "clang/Rewrite/Frontend/Rewriters.h"
#include "clang/Rewrite/Core/Rewriter.h"

#include <fstream>
#include <sstream>
#include <cstdio>

using namespace clang::tooling;
using namespace llvm;

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::ast_matchers::internal;

using namespace std;

namespace clang{
  namespace ast_matchers{
    //Zona para definir matchers respecto a métodos incluidos en las clases.
    
    /*AST_MATCHER(clang::CXXConstructorDecl, isCopyConstructor) {
      return Node.isCopyConstructor();
      }
      
      AST_MATCHER(clang::CXXMethodDecl, isCopyAssignmentOperator) {
      return Node.isCopyAssignmentOperator();
      }
      
      AST_MATCHER(clang::CXXRecordDecl, hasCopyAssignmentWithConstParam) {
      return Node.hasCopyAssignmentWithConstParam();
      }
      
      AST_MATCHER(clang::CXXRecordDecl, hasCopyConstructorWithConstParam) {
      return Node.hasCopyConstructorWithConstParam();
      }
      
      AST_MATCHER(clang::FunctionDecl, isDeleted) {
      return Node.isDeleted();
      }*/
    
    AST_MATCHER(clang::FunctionDecl, isImplicitFunction) {
      return Node.isImplicit();
    }		
	
    AST_MATCHER(clang::CXXMethodDecl, isImplicitMethod) {
      return Node.isImplicit();
    }		

    AST_MATCHER(clang::CXXConstructorDecl, hasBodyConstructor) {
      return Node.hasBody();
    }
		
    //const internal::VariadicDynCastAllOfMatcher<Decl, TranslationUnitDecl> translationUnitDecl;

    //const internal::VariadicDynCastAllOfMatcher<Decl, TypedefDecl> typedefDecl;
  }
}

/*****************************************************************************/

DeclarationMatcher Clase_Articulo_Matcher =
  recordDecl(
	     hasName("Articulo")
	     ).bind("clase_articulo");

DeclarationMatcher Clase_Tarjeta_Matcher =
  recordDecl(
	     hasName("Tarjeta")
	     ).bind("clase_tarjeta");

DeclarationMatcher Clase_Usuario_Matcher =
  recordDecl(
	     hasName("Usuario")
	     ).bind("clase_usuario");

DeclarationMatcher Clase_Pedido_Matcher =
  recordDecl(
	     hasName("Pedido")
	     ).bind("clase_pedido");

DeclarationMatcher Clase_PedidoArticulo_Matcher =
  translationUnitDecl(
		      allOf(
			    hasDescendant(
					  recordDecl(
						     hasName("Pedido_Articulo")
						     ).bind("clase_pedidoarticulo")
					  ),
			    hasDescendant(
					  recordDecl(
						     hasName("LineaPedido")
						     ).bind("clase_lineapedido")
					  )
			    )
		      );

DeclarationMatcher Clase_UsuarioPedido_Matcher =
  recordDecl(
	     hasName("Usuario_Pedido")
	     ).bind("clase_usuariopedido");


/*DeclarationMatcher Copy_Constructor_Tarjeta_Matcher =
  recordDecl(
  has( 
  constructorDecl(
  isCopyConstructor(),
  ofClass(recordDecl(hasCopyConstructorWithConstParam())),
  isDeleted()
  ).bind("copy_constructor_tarjeta")
  ),
  hasName("Tarjeta")
  );

  DeclarationMatcher Copy_Assignment_Tarjeta_Matcher =
  recordDecl(
  has( 
  methodDecl(
  isCopyAssignmentOperator(),
  ofClass(recordDecl(hasCopyAssignmentWithConstParam())),	
  isDeleted()
  ).bind("copy_assignment_tarjeta")
  ),
  hasName("Tarjeta")
  );

  DeclarationMatcher Copy_Constructor_Usuario_Matcher =
  recordDecl(
  has( 
  constructorDecl(
  isCopyConstructor(),
  ofClass(recordDecl(hasCopyConstructorWithConstParam())),	
  isDeleted()
  ).bind("copy_constructor_usuario")
  ),
  hasName("Usuario")
  );

  DeclarationMatcher Copy_Assignment_Usuario_Matcher =
  recordDecl(
  has( 
  methodDecl(
  isCopyAssignmentOperator(),
  ofClass(recordDecl(hasCopyAssignmentWithConstParam())),	
  isDeleted()
  ).bind("copy_assignment_usuario")
  ),
  hasName("Usuario")
  );*/

DeclarationMatcher One_Constructor_Usuario_Matcher =
  recordDecl(
	     forEach(
		     constructorDecl(hasBodyConstructor(), 
				     unless(isImplicit()
					    )
				     )
		     .bind("one_constructor")
		     ),
	     hasName("Usuario")
	     );


DeclarationMatcher One_Constructor_Pedido_Matcher =
  recordDecl(
	     forEach(
		     constructorDecl(
				     hasBodyConstructor(), 
				     unless(isImplicit())
				     ).bind("constructor_pedido")
		     ),
	     hasName("Pedido")
	     );

DeclarationMatcher Actualiza_Numero_Pedidos_Matcher =
  constructorDecl(
		  hasDescendant(
				compoundStmt(
					     has(
						 stmt(
						      anyOf(
							    unaryOperator(
									  hasOperatorName("++"),
									  has(
									      declRefExpr(
											  to(
											     varDecl(
												     hasName("N_pedidos")
												     )
											     )
											  )
									      )   
									  ),		   
							    binaryOperator(
									   hasOperatorName("="),
									   hasLHS(
										  declRefExpr(
											      to(
												 varDecl(
													 hasName("N_pedidos")
													 )
												 )
											      )
										  ),
									   hasRHS(
										  binaryOperator(
												 hasOperatorName("+"),
												 hasLHS(
													has(
													    declRefExpr(
															to(
															   varDecl(
																   hasName("N_pedidos")
																   )
															   )
															)
													    )  
													),
												 hasRHS(integerLiteral(equals(1)))
												 )
										  )
									   ),
							    binaryOperator(
									   hasOperatorName("="),
									   hasRHS(
										  binaryOperator(
												 hasOperatorName("+"),
												 hasRHS(
													has(
													    declRefExpr(
															to(
															   varDecl(
																   hasName("N_pedidos")
																   )
															   )
															)
													    )
													),
												 hasLHS(integerLiteral(equals(1)))
												 )
										  ),
									   hasLHS(
										  declRefExpr(
											      to(
												 varDecl(
													 hasName("N_pedidos")
													 )
												 )
											      )
										  )
									   ),
							    binaryOperator(
									   hasOperatorName("+="),
									   hasLHS(
										  declRefExpr(
											      to(
												 varDecl(
													 hasName("N_pedidos")
													 )
												 )
											      )
										  ),
									   hasRHS(integerLiteral(equals(1)))  
									   )
							    )
						      ).bind("stmt_found")	
						 )
					     ).bind("constructor_pedido_compSt")
				),
		  ofClass(
			  recordDecl(
				     hasName("Pedido")
				     )
			  )
		  );

DeclarationMatcher One_Constructor_LineaPedido_Matcher =
  recordDecl(
	     forEach(
		     constructorDecl(hasBodyConstructor(), unless(isImplicit())).bind("constructor_lineapedido")
		     ),
	     hasName("LineaPedido")
	     );

DeclarationMatcher Non_Constructor_Pedido_Matcher =
  recordDecl(
	     has(
		 constructorDecl(hasBodyConstructor(), unless(isImplicit())).bind("const_pedidoarticulo")
		 ),
	     hasName("Pedido_Articulo")
	     );

DeclarationMatcher Funciones_Estandar_Matcher =
  functionDecl(
	       unless(isImplicitFunction()),
	       forEachDescendant(
				 declRefExpr(
					     to(
						functionDecl(
							     anyOf(
								   hasName("strlen"),
								   hasName("strcat"),
								   hasName("memset"),
								   hasName("strcpy"),
								   hasName("strcmp")
								   )
							     ).bind("method_cstring")
						)
					     )
				 )
	       );

DeclarationMatcher Friend_Matcher =
  recordDecl(
	     hasDescendant(
			   friendDecl(                                
				      unless(
					     has(
						 functionDecl(
							      hasName("operator<<"),
							      hasAncestor(recordDecl(hasName("Usuario")))
							      ).bind("friend_operator_usuario")
						 )	
					     )
								      )
			   )
	     ).bind("friend_class");

DeclarationMatcher Friend_Operator_Usuario_Matcher = 
  recordDecl(
	     hasDescendant(
			   friendDecl(
				      has(
					  functionDecl(
						       hasName("operator<<")
						       ).bind("friend_operator_usuario")
					  )
				      )
			   )
	     );

DeclarationMatcher Private_Fields_Matcher =
  recordDecl(
	     forEach(
		     fieldDecl().bind("field_decl")
		     )
	     );

DeclarationMatcher Typedef_Matcher =
  recordDecl(
	     forEach(
		     typedefDecl().bind("typedef_decl")
		     )
	     );


DeclarationMatcher Non_Member_Pedido_Matcher =
  methodDecl(
	     hasOverloadedOperatorName("<<"),
	     unless(isImplicitMethod()),
	     ofClass(
		     recordDecl(
				hasName("Pedido"),
				unless(isTemplateInstantiation())
				)
		     )
	     ).bind("member_operator_pedido");

DeclarationMatcher Tarjeta_Const_Matcher =
  recordDecl(
	     forEach(
		     fieldDecl().bind("tarjeta_const")
		     ),
	     hasName("Pedido"), 
	     unless(isTemplateInstantiation())
	     );

DeclarationMatcher Non_Member_LineaPedido_Matcher =
  methodDecl(
	     hasOverloadedOperatorName("<<"),
	     unless(isImplicitMethod()),
	     ofClass(
		     recordDecl(
				hasName("LineaPedido"),
				unless(isTemplateInstantiation())
				)
		     )
	     ).bind("member_operator_lineapedido");

DeclarationMatcher Non_Member_PedidoArticulo_Matcher =
  methodDecl(
	     hasOverloadedOperatorName("<<"),
	     unless(isImplicitMethod()),
	     ofClass(
		     recordDecl(
				hasName("Pedido_Articulo"),
				unless(isTemplateInstantiation())
				)
		     )
	     ).bind("member_operator_pedidoarticulo");

DeclarationMatcher Overloaded_Pedido_Matcher =
  recordDecl(
	     has(
		 methodDecl(
			    hasName("pedir")
			    ).bind("metodo_pedir")
		 ),
	     hasName("Pedido_Articulo")
	     ).bind("clase_metodo_pedir");

/*****************************************************************************/

class CompraCheck : public MatchFinder::MatchCallback {
public :
  CompraCheck(): clase_encontrada(false),
		 /*copy_constructor(false), copy_assignment(false),*/ 
		 one_constructor(0), constructor_correcto(false), 
		 constante_tarjeta(false), constructor_pedidoarticulo(false),
		 cstring(true), friend_member(false), 
		 atributo_no_privado(false), typedef_no_publico(false), 
		 friend_operator_usuario(false), explicit_specified(false),
 		 pedir_sobrecargado(false), incrementa_pedidos(false) {

    operadores_externos.insert(pair<string, bool>("operator<<", false));

    atributos.insert(pair<string, bool>("tarjeta_", false));
    atributos.insert(pair<string, bool>("num_", false));
    atributos.insert(pair<string, bool>("fecha_", false));
    atributos.insert(pair<string, bool>("total_", false));
  }

  virtual void run(const MatchFinder::MatchResult &Result) {
    Context = Result.Context;
    Rewrite.setSourceMgr(Context->getSourceManager(), Context->getLangOpts());

    if(!clase_encontrada)
      apply_clase(Result);
    else{
      apply_busqueda_cstring(Result);
      apply_friend(Result);
      apply_acceso_atributo(Result);
      apply_acceso_typedef(Result);

      if(isFileArticulo()){

      }
      else if(isFileTarjeta()){
	/*apply_copy_constructor_tarjeta(Result);
	  apply_copy_assignment_tarjeta(Result);*/
      }
      else if(isFileUsuario()){
	/*apply_copy_constructor_usuario(Result);
	  apply_copy_assignment_usuario(Result);*/
	apply_one_constructor(Result);
	apply_friend_operator(Result);
      }
      if(isFilePedido()){
	apply_constructor_correcto(Result);
	apply_operadores_externos(Result);
	apply_tarjeta_constante(Result);
	apply_pedido_incrementado(Result);
      }
      else if(isFilePedidoArticulo()){
	apply_constructor_correcto(Result);
	apply_operadores_externos(Result);
	apply_constructor_pedidoarticulo(Result);
	apply_pedir_sobrecargado(Result);
      }
      else if(isFileUsuarioPedido()){

      }
    }
  }
  
  /*bool tiene_delete_copy_constructor(){
    return copy_constructor && copy_assignment;
    }*/

  bool is_clase_encontrada(){
    return clase_encontrada;
  }

  bool tiene_one_constructor(){
    return one_constructor == 1 ? true : false;
  }

  bool tiene_constructor_pedidoarticulo(){
    return constructor_pedidoarticulo;	
  }

  bool encuentra_cstring(){
    return cstring;
  }

  bool existe_member_friend(){
    return friend_member;
  }

  bool tiene_atributo_no_privado(){
    return atributo_no_privado;
  }

  bool tiene_typedef_no_publico(){
    return typedef_no_publico;
  } 

  bool tiene_friend_operator_usuario(){
    return friend_operator_usuario;
  } 

  bool son_externos_operadores(){
    for (map<string, bool>::iterator it=operadores_externos.begin(); it!=operadores_externos.end(); it++){
      if( it->second )
	return false;    
    }
    return true;
  }

  bool is_explicit_specified(){
    return explicit_specified;
  }

  bool tiene_constructor_correcto(){
    return constructor_correcto;
  }

  bool es_constante_tarjeta(){
    return constante_tarjeta;
  }

  bool esta_pedir_sobrecargado(){
    return pedir_sobrecargado;
  }

  bool incrementa_numero_pedidos(){
    return incrementa_pedidos;
  }

  void setClase(string s){
    this->clase = s;
  }

  bool lista_inicializacion_correcta(){
    for (map<string, bool>::iterator it=atributos.begin(); it!=atributos.end(); it++){
      if( it->second )
	return false;    
    }
    return true;
  }

private: 

  void apply_clase(const MatchFinder::MatchResult &Result){
	
    if (Result.Nodes.getNodeAs<clang::CXXRecordDecl>("clase_articulo") && isFileArticulo()){
      clase_encontrada = true;
    }
    else if(Result.Nodes.getNodeAs<clang::CXXRecordDecl>("clase_tarjeta") && isFileTarjeta()){
      clase_encontrada = true;
    }
    else if(Result.Nodes.getNodeAs<clang::CXXRecordDecl>("clase_usuario") && isFileUsuario()){
      clase_encontrada = true;
    }
    else if(Result.Nodes.getNodeAs<clang::CXXRecordDecl>("clase_pedido") && isFilePedido()){
      clase_encontrada = true;
    }
    else if(Result.Nodes.getNodeAs<clang::CXXRecordDecl>("clase_pedidoarticulo") && 
	    Result.Nodes.getNodeAs<clang::CXXRecordDecl>("clase_lineapedido") && isFilePedidoArticulo()){
      clase_encontrada = true;
    }
    else if(Result.Nodes.getNodeAs<clang::CXXRecordDecl>("clase_usuariopedido") && isFileUsuarioPedido()){
      clase_encontrada = true;
    }
  }

  /*void apply_copy_constructor_tarjeta(const MatchFinder::MatchResult &Result){
    if (Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("copy_constructor_tarjeta")){
    copy_constructor = true;
    }
    }

    void apply_copy_assignment_tarjeta(const MatchFinder::MatchResult &Result){
    if (Result.Nodes.getNodeAs<clang::CXXMethodDecl>("copy_assignment_tarjeta")){
    copy_assignment = true;
    }
    }

    void apply_copy_constructor_usuario(const MatchFinder::MatchResult &Result){
    if (Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("copy_constructor_usuario")){
    copy_constructor = true;
    }
    }

    void apply_copy_assignment_usuario(const MatchFinder::MatchResult &Result){
    if (Result.Nodes.getNodeAs<clang::CXXMethodDecl>("copy_assignment_usuario")){
    copy_assignment = true;
    }
    }*/

  void apply_one_constructor(const MatchFinder::MatchResult &Result){
    if (Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("one_constructor")){
      one_constructor++;
    }
  }

  void apply_constructor_correcto(const MatchFinder::MatchResult &Result){
		
    //Solo entrara para la declaración de la clase, pero no para redeclaraciones fuera de la clase
    if (const CXXConstructorDecl *FS = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("constructor_pedido")){

      one_constructor++;

      if(one_constructor == 1 && FS->getNumParams() == 5 && !FS->getParamDecl(3)->hasDefaultArg() && FS->getParamDecl(4)->hasDefaultArg()){
	constructor_correcto = true;
      }
      else
	constructor_correcto = false;

      //Así tomamos la definición para hacer la comprobación de los inicializadores
      const CXXConstructorDecl *FS2;
      if((CXXConstructorDecl*)FS->getMostRecentDecl() != NULL){
	FS2 = (CXXConstructorDecl*)FS->getMostRecentDecl();
      }
      else
	FS2 = FS;

      if(FS2->getNumCtorInitializers() == 4){	
	for(CXXConstructorDecl::init_const_iterator i = FS2->init_begin(); i != FS2->init_end(); ++i){
	  if((*i)->isWritten() && (*i)->isMemberInitializer()){
	    FieldDecl *fd = (*i)->getMember(); 
	    map<string, bool>::iterator it;
	    if( (it = atributos.find(fd->getNameAsString())) != atributos.end() ){

	      /*if(fd->getNameAsString() == "total_"){
	      //Comprobar que es 0
	      it->second = true;
	      }
	      else if(fd->getNameAsString() == "num_"){
	      //Comprobar que es la expresión binario N_pedidos + 1 
	      it->second = true;
	      }
	      else*/
	      it->second = true;
	    }
	  }
	}
      }
    }
    else if(const CXXConstructorDecl *FS = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("constructor_lineapedido")){

      one_constructor++;

      if(one_constructor == 1 && FS->getNumParams() == 2 && !FS->getParamDecl(0)->hasDefaultArg() && FS->getParamDecl(1)->hasDefaultArg()){
	constructor_correcto = true;
      }
      else
	constructor_correcto = false;

      if(FS->isExplicitSpecified()){
	explicit_specified = true;
      }
    }	
  }

  void apply_pedido_incrementado(const MatchFinder::MatchResult &Result){

    if(const CompoundStmt *FS1 = Result.Nodes.getNodeAs<clang::CompoundStmt>("constructor_pedido_compSt")){

      const Stmt *FS3 = Result.Nodes.getNodeAs<clang::Stmt>("stmt_found");
		
      if(FS1->size() > 0){
				
	//llvm::outs() << "Stmt final IOP: " << 
	//	Rewrite.getRewrittenText(FS3->getSourceRange()) << "\n";
	const Stmt* lst = FS1->body_back();
	if(check_same_stmts(lst,FS3))
	  incrementa_pedidos = true;
      }
    }
  }

  void apply_busqueda_cstring(const MatchFinder::MatchResult &Result){
    if (const FunctionDecl *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("method_cstring")){
      FullSourceLoc FullLocation1 = Context->getFullLoc(FS->getLocStart());

      if (FullLocation1.isValid() ){
	//Si la cabecera es string.h, con esto vale. Pero si es cstring, lo que te encuentra este fullLocation es la posición de string.h 
	//dentro de cstring.h. Por eso hay que discernir en qué situación nos encontramos y si estamos en la segunda, buscar una nueva
	//localización
	FullSourceLoc FullLocation2 = Context->getFullLoc(
							  Context->getSourceManager().getIncludeLoc(FullLocation1.getFileID())
							  );

	if(FullLocation2.isValid()){
	  if( string(Context->getSourceManager().getFileEntryForID(FullLocation2.getFileID())->getName()).find("cstring") != std::string::npos){
	    FullLocation2 = Context->getFullLoc(
						Context->getSourceManager().getIncludeLoc(FullLocation2.getFileID())
						);
	  }
	}

	if(!FullLocation2.isValid() || Context->getSourceManager().isInSystemHeader(FullLocation2)){
	  cstring = false;
	}
      }
    }
  }

  void apply_friend(const MatchFinder::MatchResult &Result){
    if (const CXXRecordDecl *FS = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("friend_class")){

      FullSourceLoc FullLocation1 = Context->getFullLoc(FS->getLocStart());
      if (FullLocation1.isValid() 
	  && !Context->getSourceManager().isInSystemHeader(FullLocation1) 
	  ){
	string file = Context->getSourceManager().getFilename(FullLocation1);
	if((isFileArticulo() && file.find("articulo.h") != std::string::npos) ||
	   (isFileUsuario() && file.find("usuario.h") != std::string::npos) ||
	   (isFileTarjeta() && file.find("tarjeta.h") != std::string::npos) ||
	   (isFilePedido() && file.find("pedido.h") != std::string::npos) ||	
	   (isFilePedidoArticulo() && file.find("pedido_articulo.h") != std::string::npos) ||
	   (isFileUsuarioPedido() && file.find("usuario-pedido.h") != std::string::npos) )
	  {
	    //llvm::outs () <<"Friend member: " << FS->getNameAsString() << " \n";
	    friend_member = true;
	  }
      }
    }
  } 

  void apply_acceso_atributo(const MatchFinder::MatchResult &Result){ 
    if (const FieldDecl *FS = Result.Nodes.getNodeAs<clang::FieldDecl>("field_decl")){
      FullSourceLoc FullLocation1 = Context->getFullLoc(FS->getLocStart());

      if (FullLocation1.isValid()
	  && !Context->getSourceManager().isInSystemHeader(FullLocation1)
	  ){
	//Pare evitar que informe dos veces del mismo fallo al estar varias cabeceras incluidas unas en otras, 
	//tengo que comprobar qué fichero se está analizando
	string file = Context->getSourceManager().getFilename(FullLocation1);
	if((isFileArticulo() && file.find("articulo.h") != std::string::npos) ||
	   (isFileUsuario() && file.find("usuario.h") != std::string::npos) ||
	   (isFileTarjeta() && file.find("tarjeta.h") != std::string::npos) ||
	   (isFilePedido() && file.find("pedido.h") != std::string::npos) ||	
	   (isFilePedidoArticulo() && file.find("pedido_articulo.h") != std::string::npos) ||
	   (isFileUsuarioPedido() && file.find("usuario-pedido.h") != std::string::npos) )
	  {
	    //llvm::outs() << FS->getNameAsString() << "\n";
	    if(FS->getAccess() != AS_private){
	      atributo_no_privado = true;
	    }
	  }
      }
    }
  }

  void apply_acceso_typedef(const MatchFinder::MatchResult &Result){ 
    if (const TypedefDecl *FS = Result.Nodes.getNodeAs<clang::TypedefDecl>("typedef_decl")){
      FullSourceLoc FullLocation1 = Context->getFullLoc(FS->getLocStart());

      if (FullLocation1.isValid()
	  && !Context->getSourceManager().isInSystemHeader(FullLocation1)
	  ){
	//Pare evitar que informe dos veces del mismo fallo al estar varias cabeceras incluidas unas en otras, 
	//tengo que comprobar qué fichero se está analizando
	string file = Context->getSourceManager().getFilename(FullLocation1);
	if((isFileArticulo() && file.find("articulo.h") != std::string::npos) ||
	   (isFileUsuario() && file.find("usuario.h") != std::string::npos) ||
	   (isFileTarjeta() && file.find("tarjeta.h") != std::string::npos) ||
	   (isFilePedido() && file.find("pedido.h") != std::string::npos) ||	
	   (isFilePedidoArticulo() && file.find("pedido_articulo.h") != std::string::npos) ||
	   (isFileUsuarioPedido() && file.find("usuario-pedido.h") != std::string::npos) )
	  {
	    //llvm::outs() << FS->getNameAsString() << "\n";
	    if(FS->getAccess() != AS_public){
	      typedef_no_publico = true;
	    }
	  }
      }
    }
  }

  void apply_friend_operator(const MatchFinder::MatchResult &Result){
    if (/*const FunctionDecl *FS = */Result.Nodes.getNodeAs<clang::FunctionDecl>("friend_operator_usuario")){
      //llvm::outs() << "Entro friend operator\n";
      friend_operator_usuario = true;
    }
  }

  void apply_operadores_externos(const MatchFinder::MatchResult &Result){
    CXXMethodDecl *FS;
    if ( (FS = const_cast<CXXMethodDecl*>(Result.Nodes.getNodeAs<clang::CXXMethodDecl>("member_operator_pedido"))) ||
	 (FS = const_cast<CXXMethodDecl*>(Result.Nodes.getNodeAs<clang::CXXMethodDecl>("member_operator_pedidoarticulo"))) ||
	 (FS = const_cast<CXXMethodDecl*>(Result.Nodes.getNodeAs<clang::CXXMethodDecl>("member_operator_lineapedido"))) ){
	
      map<string, bool>::iterator it;
      if( (it = operadores_externos.find(FS->getNameAsString())) != operadores_externos.end() ){
	it->second = true;
      }
    }
  }

  void apply_tarjeta_constante(const MatchFinder::MatchResult &Result){
    if (const FieldDecl *FS = Result.Nodes.getNodeAs<clang::FieldDecl>("tarjeta_const")){
      QualType ft =  FS->getType().getCanonicalType();
      if(ft->isPointerType()){
	ft = ft->getPointeeType();
			
	if(ft.isConstQualified())
	  constante_tarjeta = true;
      }
    }
  }

  void apply_constructor_pedidoarticulo(const MatchFinder::MatchResult &Result){

    if(Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("const_pedidoarticulo")){
      constructor_pedidoarticulo = true;
    }
  } 

  void apply_pedir_sobrecargado(const MatchFinder::MatchResult &Result){

    if (const CXXMethodDecl *FS = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("metodo_pedir")){

      const CXXRecordDecl *FS2 = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("clase_metodo_pedir");

      for(CXXRecordDecl::method_iterator me = FS2->method_begin(); me != FS2->method_end(); me++){

	CXXMethodDecl *m = *me;

	if( !check_same_decl_canonical(FS, m) && 
	    (m->getNameAsString() == FS->getNameAsString()) && (m->getNumParams() == FS->getNumParams())){

	  bool proceed = true;
	  for(unsigned int p = 0; p < FS->getNumParams(); p++){

	    if(p == 0){
	      if(m->getParamDecl(p)->getType().getCanonicalType().getAsString() != FS->getParamDecl(p+1)->getType().getCanonicalType().getAsString()){
		proceed = false; break;			
	      }
	    }
	    else if(p == 1){
	      if(m->getParamDecl(p-1)->getType().getCanonicalType().getAsString() != FS->getParamDecl(p)->getType().getCanonicalType().getAsString()){
		proceed = false; break;		
	      }
	    }
	    else{
	      if(m->getParamDecl(p)->getType().getCanonicalType().getAsString() != FS->getParamDecl(p)->getType().getCanonicalType().getAsString()){
		proceed = false; break;		
	      }
	    }
	  }

	  if(proceed)
	    pedir_sobrecargado = true;

	  //Solo debería haber un método sobrecargado, por lo que sino es este, ya no debe haber ningún otro
	  break;
	}
      }
    }
  }

  bool check_same_decl_canonical(const Decl *c1, const Decl *c2){
    return c1 && c2 && c1->getCanonicalDecl() == c2->getCanonicalDecl();
  }

  bool check_same_stmts(const Stmt *First, const Stmt *Second) {
    return First && Second && First == Second;
  }

  bool isFileArticulo(){
    return clase == "Articulo";
  }

  bool isFileTarjeta(){
    return clase == "Tarjeta";
  }

  bool isFileUsuario(){
    return clase == "Usuario";
  }

  bool isFilePedido(){
    return clase == "Pedido";
  }

  bool isFilePedidoArticulo(){
    return clase == "Pedido_Articulo y LineaPedido";
  }

  bool isFileUsuarioPedido(){
    return clase == "Usuario_Pedido";
  }

  bool clase_encontrada;
  //bool copy_constructor;
  //bool copy_assignment;
  unsigned one_constructor;
  bool constructor_correcto;
  bool constante_tarjeta;
  bool constructor_pedidoarticulo;
  bool cstring;
  bool friend_member;
  bool atributo_no_privado;
  bool typedef_no_publico;
  bool friend_operator_usuario;
  map<string, bool> operadores_externos;
  bool explicit_specified;
  map<string, bool> atributos;
  bool pedir_sobrecargado;
  bool incrementa_pedidos;
 
  string clase;
  ASTContext *Context;
  Rewriter Rewrite;
};	

list<string> cabeceras_leidas(string file_name, string orden){

  int res = system(orden.c_str());
  ifstream read_file;

  read_file.open(file_name.c_str(), std::ifstream::in);
  list<string> lstr;

  if (read_file.is_open() && res ==0) {
    string s;
    getline(read_file, s);

    while (! read_file.eof()) {
      if(s != ""){
	std::size_t first = s.find_first_not_of(' ');
	s = s.substr(first);
	lstr.push_back(string("-isystem" + s));
      }
      getline(read_file, s);
    }
    read_file.close();
  }
  return lstr;
}

static llvm::cl::OptionCategory MyToolCategory("pedido_check options");

int main(int argc, const char **argv) {

  string option = argv[1];

  if(option.compare("--help") == 0){
    llvm::outs() << "Comando: ./pedido_check articulo.cpp tarjeta.cpp usuario.cpp pedido.cpp pedido-articulo.cpp usuario-pedido.h -- -std=c++11\n";
    exit(EXIT_SUCCESS);
  }

  int fixed = 0;
  //  int arg_std = 0;
  for(int i = 2; i < argc; i++){	//argv[0] is "mucpp" and argv[1] is the option, so we begin in 2
    string arg = argv[i];
    if(arg.compare("--")==0){
      fixed = argc - i;		
    }
    /*if(arg.compare("-std=c++11") == 0){
      arg_std = argc - 1;
      //llvm::outs() << argv[arg_std] << "\n";
      }*/
  }
 
  string file= "";
  unsigned startSources = 1;
  list<string> listSources;
  list<string>::iterator it;

  while(argc - fixed > int(startSources)){
    file = (string(argv[startSources]));

    /*if(file.find("usuario-pedido.h") != std::string::npos){
    //Cambiar .h por .hpp
    file+= "pp";

    ifstream source("usuario-pedido.h", ios::binary);
    ofstream dest("usuario-pedido.hpp", ios::binary);

    dest << source.rdbuf();

    source.close();
    dest.close();
    }*/	
	
    it = find(listSources.begin(), listSources.end(), file);
    if(listSources.empty() || it == listSources.end()){
      listSources.push_back(file);
    }

    startSources++;
  }

  string file_name = "pedido_check.tmp";
  string orden =  "echo | clang -E -v -x c++ - 2>&1 | sed -n '/<...> search starts here/,/End of search list./p' | sed 's/End of search list.//g' | sed 's/#include <...> search starts here://g' >" + file_name;

  list<string> lstr = cabeceras_leidas(file_name, orden);

  orden =  "echo | LANG=C cpp -v -x c++ - 2>&1 | sed -n '/<...> search starts here/,/End of search list./p' | sed 's/End of search list.//g' | sed 's/#include <...> search starts here://g' >" + file_name;

  list<string> lstr_clang = cabeceras_leidas(file_name, orden);
  lstr.insert(lstr.end(), lstr_clang.begin(), lstr_clang.end());

  for(auto pos = lstr.begin(); pos != lstr.end(); pos++){
    argv[argc] = (*pos).c_str();
    //llvm::outs() << argv[argc] << " ";
    argc++;
  }

  //for(int i=0; i< argc; i++)
  //    llvm::outs() << argv[i] << " ";

  orden = "rm " + file_name  + " > /dev/null 2>&1";

  system(orden.c_str());
 
  bool sin_fallos;
  CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
  for(auto pos = listSources.begin(); pos != listSources.end(); pos++){

    CompraCheck FC;	
    ClangTool Tool(OptionsParser.getCompilations(), *pos);
    MatchFinder FinderClase;
    MatchFinder Finder;
    string clase = "";

    //General
    Finder.addMatcher(Funciones_Estandar_Matcher, &FC);
    Finder.addMatcher(Friend_Matcher, &FC);
    Finder.addMatcher(Private_Fields_Matcher, &FC);
    //Finder.addMatcher(Typedef_Matcher, &FC);

    if(*pos == "articulo.cpp"){
      FinderClase.addMatcher(Clase_Articulo_Matcher, &FC);
      clase = "Articulo";
      FC.setClase(clase);
      Tool.run(newFrontendActionFactory(&FinderClase).get());

      llvm::outs() << "* articulo.cpp: ";

      if(FC.is_clase_encontrada()){
	sin_fallos = true;

	Tool.run(newFrontendActionFactory(&Finder).get());
      }
      else
	llvm::outs() << "No se ha encontrado la clase 'Artículo'.\n";
    }
    else if(*pos == "tarjeta.cpp"){
      FinderClase.addMatcher(Clase_Tarjeta_Matcher, &FC);
      clase = "Tarjeta";
      FC.setClase(clase);
      Tool.run(newFrontendActionFactory(&FinderClase).get());

      llvm::outs() << "* tarjeta.cpp: ";
		
      if(FC.is_clase_encontrada()){
	sin_fallos = true;
	// 			Finder.addMatcher(Copy_Constructor_Tarjeta_Matcher, &FC);
	//			Finder.addMatcher(Copy_Assignment_Tarjeta_Matcher, &FC);

	Tool.run(newFrontendActionFactory(&Finder).get());
		
	/*			if(!FC.tiene_delete_copy_constructor()){
				sin_fallos = false;
				llvm::outs() << "Revisa el enunciado respecto a la copia de objetos.\n";
				}	*/
      }
      else
	llvm::outs() << "No se ha encontrado la clase 'Tarjeta'.\n";
    }
    else if(*pos == "usuario.cpp"){
      FinderClase.addMatcher(Clase_Usuario_Matcher, &FC);
      clase = "Usuario";
      FC.setClase(clase);
      Tool.run(newFrontendActionFactory(&FinderClase).get());

      llvm::outs() << "* usuario.cpp: ";
		
      if(FC.is_clase_encontrada()){

	sin_fallos = true;
	// 			Finder.addMatcher(Copy_Constructor_Usuario_Matcher, &FC);
	//			Finder.addMatcher(Copy_Assignment_Usuario_Matcher, &FC);
	Finder.addMatcher(One_Constructor_Usuario_Matcher, &FC);
	Finder.addMatcher(Friend_Operator_Usuario_Matcher, &FC);

	Tool.run(newFrontendActionFactory(&Finder).get());

	/*			if(!FC.tiene_delete_copy_constructor()){
				sin_fallos = false;
				llvm::outs() << "Revisa el enunciado respecto a la copia de objetos.\n";
				}
	*/
	if(!FC.tiene_one_constructor()){
	  sin_fallos = false;
	  llvm::outs() << "Revisa el enunciado respecto a los construcción de objetos.\n";
	}

	if(!FC.tiene_friend_operator_usuario()){
	  sin_fallos = false;
	  llvm::outs() << "Revisa el enunciado respecto al operador de extracción.\n";
	}
      }
      else
	llvm::outs() << "No se ha encontrado la clase 'Usuario'.\n";
    }
    else if(*pos == "pedido.cpp"){
      FinderClase.addMatcher(Clase_Pedido_Matcher, &FC);
      clase = "Pedido";
      FC.setClase(clase);
      Tool.run(newFrontendActionFactory(&FinderClase).get());

      llvm::outs() << "* pedido.cpp: ";
		
      if(FC.is_clase_encontrada()){
	sin_fallos = true;
	Finder.addMatcher(One_Constructor_Pedido_Matcher, &FC);
	Finder.addMatcher(Actualiza_Numero_Pedidos_Matcher, &FC);
	Finder.addMatcher(Non_Member_Pedido_Matcher, &FC);
	Finder.addMatcher(Tarjeta_Const_Matcher, &FC);
		
	Tool.run(newFrontendActionFactory(&Finder).get());

	if(!FC.tiene_constructor_correcto()){
	  sin_fallos = false;
	  llvm::outs() << "Revisa el enunciado respecto a los construcción de objetos.\n";
	}

	if(sin_fallos && FC.lista_inicializacion_correcta()){
	  sin_fallos = false;
	  llvm::outs() << "Revisa la lista de inicialización del constructor.\n";		
	}

	if(sin_fallos && !FC.incrementa_numero_pedidos()){
	  sin_fallos = false;
	  llvm::outs() << "Revisa que el número total de pedidos quede actualizado justo al final del constructor.\n";
	}

	if(!FC.es_constante_tarjeta()){
	  sin_fallos = false;
	  llvm::outs() << "Revisa el enunciado respecto al atributo de la tarjeta de pago.\n";
	}

	if(!FC.son_externos_operadores()){
	  sin_fallos = false;
	  llvm::outs() << "Revisa el lugar de la declaración de los operadores.\n";
	}
      }
      else
	llvm::outs() << "No se ha encontrado la clase 'Pedido'.\n";
    }
    else if(*pos == "pedido-articulo.cpp"){
      FinderClase.addMatcher(Clase_PedidoArticulo_Matcher, &FC);
      clase = "Pedido_Articulo y LineaPedido";
      FC.setClase(clase);
      Tool.run(newFrontendActionFactory(&FinderClase).get());

      llvm::outs() << "* pedido-articulo.cpp: ";
		
      if(FC.is_clase_encontrada()){
	sin_fallos = true;
	Finder.addMatcher(Non_Member_PedidoArticulo_Matcher, &FC);
	Finder.addMatcher(Non_Member_LineaPedido_Matcher, &FC);
	Finder.addMatcher(One_Constructor_LineaPedido_Matcher, &FC);
	Finder.addMatcher(Non_Constructor_Pedido_Matcher, &FC);
	Finder.addMatcher(Overloaded_Pedido_Matcher, &FC);

	Tool.run(newFrontendActionFactory(&Finder).get());

	if(!FC.tiene_constructor_correcto()){
	  sin_fallos = false;
	  llvm::outs() << "Revisa el enunciado respecto a los construcción de objetos.\n";
	}

	if(sin_fallos && !FC.is_explicit_specified()){
	  sin_fallos = false;
	  llvm::outs() << "Revisa el enunciado respecto a conversiones implícitas.\n";
	}

	if(FC.tiene_constructor_pedidoarticulo()){
	  sin_fallos = false;
	  llvm::outs() << "Revisa la necesidad de definir constructores.\n";
	}

	if(!FC.son_externos_operadores()){
	  sin_fallos = false;
	  llvm::outs() << "Revisa el lugar de la declaración de los operadores.\n";
	}

	if(!FC.esta_pedir_sobrecargado()){
	  sin_fallos = false;
	  llvm::outs() << "Se sugiere la sobrecarga del método 'pedir'.\n";
	}
      }
      else
	llvm::outs() << "No se ha encontrado la clase 'Pedido_Articulo' o 'LineaPedido'.\n";
    }
    else if(*pos == "usuario-pedido.hpp"){
      FinderClase.addMatcher(Clase_UsuarioPedido_Matcher, &FC);
      clase = "Usuario_Pedido";
      FC.setClase(clase);

      Tool.run(newFrontendActionFactory(&FinderClase).get());

      llvm::outs() << "* usuario-pedido.hpp: ";
		
      if(FC.is_clase_encontrada()){
	sin_fallos = true;

	Tool.run(newFrontendActionFactory(&Finder).get());
      }
      else
	llvm::outs() << "No se ha encontrado la clase 'Usuario_Pedido'.\n";
    }

    //General
    if(FC.is_clase_encontrada()){
      //llvm::outs() << "-------------\n";
      if(!FC.encuentra_cstring()){
	sin_fallos = false;
	llvm::outs() << "Revisa de dónde son tomadas las funciones de la biblioteca estándar como strlen, strcpy...\n";
      }

      if(FC.existe_member_friend()){
	sin_fallos = false;
	llvm::outs() << "Revisa por qué es necesario incluir 'friend'.\n";
      }
	
      if(FC.tiene_atributo_no_privado()){
	sin_fallos = false;
	llvm::outs() << "Revisa el acceso a los atributos.\n";
      }

      if(FC.tiene_typedef_no_publico()){
	sin_fallos = false;
	llvm::outs() << "Revisa el acceso a los tipos definidos.\n";
      }

      if(sin_fallos)
	llvm::outs() << "Verificación correcta de la clase " << clase  << ".\n";
    }
    //if(sin_fallos)
    //	llvm::outs() << "Verificación completa de compra.\n";
  }

  /*  string orden_clean = "rm usuario-pedido.hpp";
      system(orden_clean.c_str());*/

  exit(EXIT_SUCCESS);
}
