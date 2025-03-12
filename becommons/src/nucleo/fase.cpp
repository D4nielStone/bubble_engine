#include "nucleo/fase.hpp"
#include "nucleo/projeto.hpp"
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <filesystem>
#include "componentes/codigo.hpp"
#include "componentes/texto.hpp"
#include "componentes/transformacao.hpp"
#include "componentes/terreno.hpp"
#include "componentes/propriedades.hpp"
#include "componentes/fisica.hpp"
#include "componentes/renderizador.hpp"
#include "componentes/imagem.hpp"
#include "componentes/luz_direcional.hpp"
#include "os/janela.hpp"
#include "os/sistema.hpp"
#include <iostream>

using namespace rapidjson;
using namespace bubble;

fase::fase() : _Mnome("")
{
}

fase::~fase()
{
}

void fase::carregar()
{
	srender.inicializar(this);
	sinterface.inicializar(this);

	/// efetua a analise do json
	projeto_atual->fase_atual = this;
}

fase::fase(const char* diretorio) : diretorio(diretorio)
{
	if(std::filesystem::exists(diretorio))
		analizar(diretorio);
	else if (std::filesystem::exists(std::filesystem::absolute(diretorio)))
	{
		
		analizar(std::filesystem::absolute(diretorio).string().c_str());
	}
}

fase::fase(const std::string& diretorio) : diretorio(diretorio.c_str())
{
	srender.inicializar(this);
	sinterface.inicializar(this);

	/// efetua a analise do json
	projeto_atual->fase_atual = this;

	if(std::filesystem::exists(diretorio))
		analizar(diretorio.c_str());
	else if (std::filesystem::exists(std::filesystem::absolute(diretorio)))
	{
		
		analizar(std::filesystem::absolute(diretorio).string().c_str());
	}
}

static void analizarMalha(bubble::malha* m, const rapidjson::Value& malha)
{
	/// cor difusa

	if (malha.HasMember("cor_difusa"))
		m->material.difusa =
	{
		malha["cor_difusa"].GetArray()[0].GetFloat() / 255,
		malha["cor_difusa"].GetArray()[1].GetFloat() / 255,
		malha["cor_difusa"].GetArray()[2].GetFloat() / 255,
		malha["cor_difusa"].GetArray()[3].GetFloat() / 255,
	};
	/// recebe luz
	if (malha.HasMember("recebe_luz"))
		m->material.recebe_luz = malha["recebe_luz"].GetBool();
	/// sobrepor
	if (malha.HasMember("sobrepor"))
		m->sobrepor = malha["sobrepor"].GetBool();
	/// itera instancias
	if (malha.HasMember("instancias"))
	{
		for (auto& instancia : malha["instancias"].GetArray())
		{
			m->instancias_pos.push_back( glm::vec3(instancia[0].GetFloat(),instancia[1].GetFloat(),instancia[2].GetFloat()));
		}
	}
}

static void analizarCamera(bubble::entidade& ent, const Value& value, bubble::fase* fase)
{
	auto reg = fase->obterRegistro();

	reg->adicionar<camera>(ent);
	fase->definirCamera(ent);
	fase->obterCamera()->viewport_ptr = &instanciaJanela->tamanho;
	if (value.HasMember("olhar"))
		reg->obter<transformacao>(ent.id)->apontarEntidade(value["apontarEntidade"].GetInt());
	if (value.HasMember("escala"))
		fase->obterCamera()->escala = value["escala"].GetFloat();
	if (value.HasMember("ortho"))
		fase->obterCamera()->flag_orth = value["ortho"].GetBool();
	if (value.HasMember("ceu"))
	{
		auto ceu = value["ceu"].GetArray();
		fase->obterCamera()->ceu =
		{
			ceu[0].GetFloat() / 255,
			ceu[1].GetFloat() / 255,
			ceu[2].GetFloat() / 255,
			ceu[3].GetFloat() / 255,
		};
	}
}

static void analizarEntidades(const Document& doc, fase* f)
{
	/// facilita acesso ao registro
	auto reg = f->obterRegistro();
	if (!(doc.HasMember("entidades") && doc["entidades"].IsArray())) return;

	for (auto& entidade : doc["entidades"].GetArray())
	{
		/// cria entidade
		auto id = reg->criar();
		/// itera os componentes
		if (entidade.HasMember("componentes") && entidade["componentes"].IsArray())
		{
			for (auto& componente : entidade["componentes"].GetArray())
			{
				if (!(componente.HasMember("tipo") && componente["tipo"].IsString())) return;
				
				const char* tipo_str = componente["tipo"].GetString();
				if (std::strcmp(tipo_str, "camera") == 0)
				{
					analizarCamera(id, componente, f);
				}
				else if (std::strcmp(tipo_str, "renderizador") == 0)
				{
					auto path = projeto_atual->diretorioDoProjeto + std::string(componente["diretorio"].GetString());
					reg->adicionar<renderizador>(id, new modelo(path.c_str()));
					auto render = reg->obter<renderizador>(id.id);
					/// extrai sahder
					if (componente.HasMember("vertex_shader") && componente.HasMember("fragment_shader"))
					{
						render->modelo->definirShader((projeto_atual->diretorioDoProjeto +componente["vertex_shader"].GetString()).c_str(), (projeto_atual->diretorioDoProjeto +componente["fragment_shader"].GetString()).c_str());
					}
					/// extrai material
					if (componente.HasMember("malhas"))
					{
						auto malhas = componente["malhas"].GetArray();
						for (auto& malha : malhas)
						{
							bubble::malha* m;
							if (malha["id"].IsInt())
							{
								m = render->modelo->obterMalha(malha["id"].GetInt());
								analizarMalha(m, malha);
							}
							else if (malha["id"].IsString())
							{
								for (auto& m : render->modelo->malhas)
								{
									analizarMalha(&m, malha);
								}
							}
						}
					}
				}
				else if (std::strcmp(tipo_str, "propriedades") == 0)
					reg->adicionar<propriedades>(id);
				else if (std::strcmp(tipo_str, "transformacao") == 0)
				{
					reg->adicionar<transformacao>(id);
					auto tr = reg->obter<transformacao>(id.id);
					auto pos = componente["posicao"].GetArray();
					tr->posicao = { pos[0].GetFloat(), pos[1].GetFloat(), pos[2].GetFloat() };
					auto rot = componente["rotacao"].GetArray();
					tr->rotacao = { rot[0].GetFloat(), rot[1].GetFloat(), rot[2].GetFloat() };
					auto esc = componente["escala"].GetArray();
					tr->escala = { esc[0].GetFloat(), esc[1].GetFloat(), esc[2].GetFloat() };
				}
				else if (std::strcmp(tipo_str, "texto") == 0)
				{
					std::string frase{};
					float size{ 1.f };
					if (componente.HasMember("frase") && componente["frase"].IsString())
						frase = componente["frase"].GetString();
					if (componente.HasMember("escala") && componente["escala"].IsFloat())
						size = componente["escala"].GetFloat();
					reg->adicionar<texto>(id, frase, size);
				}
				else if (std::strcmp(tipo_str, "codigo") == 0)
				{
					reg->adicionar<codigo>(id,projeto_atual->diretorioDoProjeto + componente["diretorio"].GetString());
				}
				else if (std::strcmp(tipo_str, "luz_direcional") == 0)
				{
					reg->adicionar<luz_direcional>(id, 
					vet3(componente["direcao"].GetArray()[0].GetFloat(),componente["direcao"].GetArray()[1].GetFloat(),componente["direcao"].GetArray()[2].GetFloat()),
					vet3(componente["ambiente"].GetArray()[0].GetFloat(),componente["ambiente"].GetArray()[1].GetFloat(),componente["ambiente"].GetArray()[2].GetFloat()),
					vet3(componente["difusa"].GetArray()[0].GetFloat(),componente["difusa"].GetArray()[1].GetFloat(),componente["difusa"].GetArray()[2].GetFloat()),
					vet3(componente["especular"].GetArray()[0].GetFloat(),componente["especular"].GetArray()[1].GetFloat(),componente["especular"].GetArray()[2].GetFloat())
					);
				}
				else if (std::strcmp(tipo_str, "imagem") == 0)
				{
					reg->adicionar<imagem>(id,projeto_atual->diretorioDoProjeto + componente["diretorio"].GetString());
					reg->obter<imagem>(id.id)->flip = componente["flip"].GetBool();
				}
				else if (std::strcmp(tipo_str, "codigo") == 0)
				{
					reg->adicionar<codigo>(id, projeto_atual->diretorioDoProjeto + componente["diretorio"].GetString());
				}
				else if (std::strcmp(tipo_str, "terreno") == 0)
				{
					reg->adicionar<terreno>(id, projeto_atual->diretorioDoProjeto + componente["textura"].GetString());
					if (componente.HasMember("textura_difusa") && componente["textura_difusa"].IsString())
					{
						bubble::textura tex;
						tex.path = projeto_atual->diretorioDoProjeto +componente["textura_difusa"].GetString();
						tex.id = bubble::textureLoader::obterInstancia().carregarTextura(tex.path, 3553U);
						tex.tipo = "texture_diffuse";
						reg->obter<terreno>(id.id)->_Mmalha.material.texturas.push_back(tex);
						reg->obter<terreno>(id.id)->_Mmalha.material.uvMundo = true;
					}
				}
				else if (std::strcmp(tipo_str, "fisica") == 0)
				{
					btCollisionShape* forma;
					auto f = componente["forma"].GetString();
					const btVector3 n(0, 1, 0);
					btVector3 posin;
					btScalar massa;
					fisica::camada camada{fisica::COLISAO_PADRAO};
					if (componente.HasMember("camada") && componente["camada"].GetInt() == 1)
						camada = fisica::COLISAO_ESPECIAL;
					if (componente.HasMember("posicao_inicial"))
						posin = {componente["posicao_inicial"].GetArray()[0].GetFloat(),componente["posicao_inicial"].GetArray()[1].GetFloat(),componente["posicao_inicial"].GetArray()[2].GetFloat()};
					if (std::strcmp(f, "esfera") == 0)
					{
						forma = new btSphereShape(componente["raio"].GetFloat());
					}
					else if(std::strcmp(f, "plano") == 0)
						forma = new btStaticPlaneShape(n, 0);
					else if(std::strcmp(f, "caixa") == 0)
						forma = new btBoxShape(btVector3(1.f, 1.f, 1.f));
					else if (std::strcmp(f, "modelo") == 0)
					{
						reg->adicionar<fisica>(id, true, massa, posin, camada);
						continue;
					}
					massa = componente["massa"].GetFloat();
					reg->adicionar<fisica>(id, forma, massa, posin, camada);
				}
				
			}
		}
	}
}

void bubble::fase::analizar(const char* diretorio)
{
	std::ifstream file(diretorio);
	std::stringstream sb;
	sb << file.rdbuf();
	Document doc;
	doc.Parse(sb.str().c_str());

	if (doc.HasParseError()) 
	{
		depuracao::emitir(erro, "Parse da fase");
	}
	/*----Analise da cena-----*/
	if (doc.HasMember("nome") && doc["nome"].IsString())
	{
		_Mnome = doc["nome"].GetString();
		depuracao::emitir(debug, "Fase", "Nome definido como " + _Mnome);
	}
	if (doc.HasMember("selecionada") && doc["selecionada"].IsBool())
	{
		if (doc["selecionada"].GetBool())
		{
			depuracao::emitir(debug, "Fase", "Fase ativa");
		}
	}
	/*------------------------*/
	analizarEntidades(doc, this);
}

void bubble::fase::pausar()
{
	file_de_tarefas.push([this]() 
		{
			depuracao::emitir(debug, "fase", "Pausando");
			rodando = false;
			//scodigo.pararThread();
			//sfisica.pararThread();
		});
}

void bubble::fase::parar()
{
	depuracao::emitir(debug, "fase", "Parando");
	// TODO: snapshot para retornar o rodando do registro
	rodando = false;
	//scodigo.pararThread();
	//sfisica.pararThread();
}

void bubble::fase::iniciar()
{
	depuracao::emitir(debug, "fase", "Iniciando");
	if (rodando != false)
		return;

	if (inicializacao)
	{
		/// o sistema de c�digo apenas inicia ao come�ar a fase
		/// no modo de joo
		inicializacao = false;
		sfisica.inicializar(this);
		scodigo.inicializar(this);
	}
	// capturar snapshot do registro
	rodando = true;
	//scodigo.iniciarThread();
	//sfisica.iniciarThread();
}

void bubble::fase::atualizar(double deltaTime)
{
	sfisica.atualizar();
	scodigo.atualizar();
	srender.atualizar(); 
	sinterface.atualizar();
	while (!file_de_tarefas.empty())
	{
		auto func = file_de_tarefas.front();
		func();
		file_de_tarefas.pop();
	}
}

void bubble::fase::definirCamera(const entidade& ent)
{
	if (reg.tem<camera>(ent.id))
		camera_atual = reg.obter<camera>(ent.id);
}

std::shared_ptr<bubble::camera> bubble::fase::obterCamera() const
{
	return camera_atual;
}

bubble::registro* bubble::fase::obterRegistro()
{
	return &reg;
}

std::string bubble::fase::nome() const
{
	return _Mnome;
}