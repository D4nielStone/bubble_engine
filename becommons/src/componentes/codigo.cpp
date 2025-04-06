#include "componentes/codigo.hpp"
#include "nucleo/fase.hpp"
#include "nucleo/projeto.hpp"
#include "api/api_lua.hpp"
#include "api/mat.hpp"
#include "inputs/inputs.hpp"
#include "os/janela.hpp"
#include "os/sistema.hpp"
#include <cmath>

bubble::codigo::codigo(const std::string& arquivo) : L(luaL_newstate()), arquivo(arquivo)

{
	luaL_openlibs(L);

	bapi::definirFisica(L);
	bapi::definirTempo(L);
	bapi::definirUtils(L);
	bapi::definirInputs(L);
	bapi::entidade::definir(L);

	if (luaL_dofile(L, arquivo.c_str()) != LUA_OK) {
        std::cerr << "Erro ao carregar o script Lua: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);  // Limpa a mensagem de erro
    }
}

void bubble::codigo::iniciar() const
{

	luabridge::setGlobal(L, new bapi::entidade(meu_objeto), "eu");
	luabridge::setGlobal(L, &projeto_atual, "projetoAtual");
	// Tentar obter a fun��o "iniciar" definida localmente no script
	lua_getglobal(L, "iniciar");
	if (lua_isfunction(L, -1)) {
	    lua_pcall(L, 0, 0, 0); // Chama a função sem argumentos e sem retorno
	} else {
	    std::cerr << "Função 'iniciar' não definida no Lua!" << std::endl;
	    if (lua_isnil(L, -1)) {
	        std::cerr << "A função 'iniciar' não foi definida, ou foi definida com valor nil." << std::endl;
	    } else if (!lua_isfunction(L, -1)) {
	        std::cerr << "A variável 'iniciar' não é uma função." << std::endl;
	    }
	}
}

bool bubble::codigo::analizar(const rapidjson::Value& value) 
{
    if(!value.HasMember("diretorio")) return false;
    arquivo = bubble::projeto_atual->diretorioDoProjeto + value["diretorio"].GetString();
	L = luaL_newstate();
	luaL_openlibs(L);

	bapi::definirFisica(L);
	bapi::definirTempo(L);
	bapi::definirUtils(L);
	bapi::definirInputs(L);
	bapi::entidade::definir(L);

	if (luaL_dofile(L, arquivo.c_str()) != LUA_OK) {
        std::cerr << "Erro ao carregar o script Lua: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);  // Limpa a mensagem de erro
    }
    return true;
};

void bubble::codigo::atualizar() const
{
	lua_getglobal(L, "atualizar");
	if (lua_isfunction(L, -1)) {
	    // Chamar a função 'atualizar' no Lua
	    lua_pcall(L, 0, 0, 0); // Chama a função sem argumentos e sem retorno
	} else {
	    std::cerr << "Função 'atualizar' não definida no Lua!" << std::endl;
	}
}
void bubble::codigo::encerrar()
{
	if (L) {
		lua_pushnil(L);
		lua_setglobal(L, "eu");
		lua_setglobal(L, "projetoAtual");
		lua_close(L);
		L = nullptr;
	}
}
bubble::codigo::~codigo()
{
        depuracao::emitir(debug, "codigo", "descarregando");

	encerrar();
}
