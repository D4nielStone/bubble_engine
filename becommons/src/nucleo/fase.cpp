#include "nucleo/fase.hpp"
#include "arquivadores/imageloader.hpp"
#include "util/malha.hpp"
#include "nucleo/projeto.hpp"
#include "os/janela.hpp"
#include "componentes/renderizador.hpp"
#include "componentes/luz_pontual.hpp"
#include "componentes/camera.hpp"
#include "componentes/codigo.hpp"
#include "componentes/terreno.hpp"
#include "componentes/luz_direcional.hpp"
#include "componentes/transformacao.hpp"
#include "componentes/texto.hpp"
#include "componentes/imagem.hpp"
#include "entidades/entidade.hpp"
#include <filesystem>
#include <iostream>
#include <typeinfo>
#include <rapidjson/document.h>

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
    if(carregada)
    descarregar();
    carregada = true;
    
    if(std::filesystem::exists(diretorio))
		analizar(diretorio);	
    else
    {
        depuracao::emitir(erro, "Erro no parsing de: " + diretorio);
    }
}

void fase::descarregar()
{
    carregada = false;
    reg.entidades.clear();
}

fase::fase(const char* diretorio) : diretorio(diretorio)
{
	
}

fase::fase(const std::string& diretorio) : diretorio(diretorio)
{
	
}

void bubble::fase::pausar()
{
			depuracao::emitir(debug, "fase", "Pausando");
			rodando = false;
}

void bubble::fase::parar()
{
	depuracao::emitir(debug, "fase", "Parando");
	// TODO: snapshot para retornar o rodando do registro
	rodando = false;
}

void bubble::fase::iniciar()
{
	depuracao::emitir(debug, "fase", "Iniciando");
	if (rodando != false)
		return;
    rodando = true;
}

bubble::registro* bubble::fase::obterRegistro()
{
	return &reg;
}

void bubble::fase::nome(const std::string& nome)
{
	_Mnome = nome;
}
std::string bubble::fase::nome() const
{
	return _Mnome;
}
static void analizarMalha(bubble::malha* m, const rapidjson::Value& malha)
{
	/// cor difusa

	if (malha.HasMember("albedo"))
		m->material.albedo =
	{
		malha["albedo"].GetArray()[0].GetFloat() / 255,
		malha["albedo"].GetArray()[1].GetFloat() / 255,
		malha["albedo"].GetArray()[2].GetFloat() / 255,
		malha["albedo"].GetArray()[3].GetFloat() / 255,
	};
	
	// textura
	if(malha.HasMember("texAlbedo"))
        {
            auto path_ = projeto_atual->diretorioDoProjeto + std::string(malha["texAlbedo"].GetString());
            bubble::textura tex = bubble::textura(bubble::textureLoader::obterInstancia().carregarTextura(path_), path_);
            m->material.texturas["tex_albedo"] = tex;
        }
	/// Uv mundo
	if (malha.HasMember("uvMundo"))
		m->material.uvMundo = malha["uvMundo"].GetBool();
	/// recebe luz
	if (malha.HasMember("recebeLuz"))
		m->material.recebe_luz = malha["recebeLuz"].GetBool();
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
	auto cam = fase->obterRegistro()->obter<camera>(ent.id);
	cam->viewport_ptr = &instanciaJanela->tamanho;
	
if(value.HasMember("fov"))
        cam->fov = value["fov"].GetFloat();
	if (value.HasMember("escala"))
if(value.HasMember("zfar"))
        cam->corte_longo = value["zfar"].GetFloat();
	if (value.HasMember("escala"))
		cam->escala = value["escala"].GetFloat();
	if (value.HasMember("ortho"))
		cam->flag_orth = value["ortho"].GetBool();
	if (value.HasMember("ceu"))
	{
		auto ceu = value["ceu"].GetArray();
		cam->ceu =
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
					auto path = projeto_atual->diretorioDoProjeto + std::string(componente["modelo"].GetString());
					reg->adicionar<renderizador>(id, new modelo(path.c_str()));
					auto render = reg->obter<renderizador>(id.id);
					

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
							else if (malha["id"].IsString() && malha["id"] == "*")
							{
								for (auto& m : render->modelo->malhas)
								{
									analizarMalha(&m, malha);
								}
							}
						}
					}
				}
				else if (std::strcmp(tipo_str, "transformacao") == 0)
				{
					auto tr = reg->obter<transformacao>(id.id);
					if(componente.HasMember("posicao"))
                    {
                        auto pos = componente["posicao"].GetArray();
					    tr->posicao = { pos[0].GetFloat(), pos[1].GetFloat(), pos[2].GetFloat() };
                    }
                    if(componente.HasMember("rotacao"))
                    {
                        auto rot = componente["rotacao"].GetArray();
					    tr->rotacao = { rot[0].GetFloat(), rot[1].GetFloat(), rot[2].GetFloat() };
                    }
                    if(componente.HasMember("escala"))
                    {
                        auto esc = componente["escala"].GetArray();
					    tr->escala = { esc[0].GetFloat(), esc[1].GetFloat(), esc[2].GetFloat() };
				
				    }
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
				else if (std::strcmp(tipo_str, "luz_pontual") == 0)
				{
					reg->adicionar<luz_pontual>(id);
				}
				else if (std::strcmp(tipo_str, "luz_direcional") == 0)
				{
					reg->adicionar<luz_direcional>(id, 
					vet3(componente["direcao"].GetArray()[0].GetFloat(),componente["direcao"].GetArray()[1].GetFloat(),componente["direcao"].GetArray()[2].GetFloat()),
					vet3(componente["ambiente"].GetArray()[0].GetFloat(),componente["ambiente"].GetArray()[1].GetFloat(),componente["ambiente"].GetArray()[2].GetFloat()),
					vet3(componente["cor"].GetArray()[0].GetFloat(),componente["cor"].GetArray()[1].GetFloat(),componente["cor"].GetArray()[2].GetFloat()),
					componente["intensidade"].GetFloat()
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
				    if(componente.HasMember("ruido"))
					reg->adicionar<terreno>(id, projeto_atual->diretorioDoProjeto + componente["ruido"].GetString());
					if (componente.HasMember("albedo") && componente["albedo"].IsString())
					{
						bubble::textura tex(
						bubble::textureLoader::obterInstancia().carregarTextura(tex.path),
						projeto_atual->diretorioDoProjeto +componente["albedo"].GetString());
						reg->obter<terreno>(id.id)->_Mmalha.material.texturas["tex_albedo"] = tex;
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

void bubble::fase::analizar(const std::string& diretorio)
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
	/*------------------------*/
	analizarEntidades(doc, this);
}
