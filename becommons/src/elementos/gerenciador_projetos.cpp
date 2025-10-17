/** \copyright
 * MIT License
 * Copyright (c) 2025 Daniel Oliveira
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "becommons/becommons.hpp"
#include "elementos/docking.hpp"
#include "elementos/gerenciador_projetos.hpp"

using namespace EDITOR_NS;

/**
 * @brief Construtor da classe gerenciador_projetos.
 * Inicializa o diretório raiz padrão onde os projetos serão armazenados.
 * @param dir O caminho do diretório raiz padrão.
 */
gerenciador_projetos::gerenciador_projetos(const std::string& dir) : painel("project manager"), DIR_PADRAO(dir) {
    buscarProjetos();
    m_estilo.m_padding_geral = {2,2};

    configCaixas();
}
void gerenciador_projetos::configCaixas() {
    m_filhos.clear();

    auto* text = adicionar<elementos::texto>("projetos encontrados:", 18, elementos::flags_texto::alinhamento_central);
    text->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual;
    text->m_estilo.m_largura = 1;
        
    auto* top = adicionar<caixa>();
    top->m_estilo.m_flag_estilo |= flag_estilo::altura_justa | flag_estilo::largura_percentual;
    top->m_estilo.m_largura = 1;

    auto* c = top->adicionar<elementos::caixa_de_texto>("Novo Projeto", &m_buffer_projeto);
    auto* btn = top->adicionar<elementos::botao>([this](){
            criarProjeto(DIR_PADRAO, m_buffer_projeto.c_str(), false);
            motor::obter().fila_opengl.push([this](){
                configCaixas();
            });
            }, "criar");
    btn->m_estilo.m_padding_geral = {2, 2};
    top->m_estilo.m_cor_fundo = cor(0.07f);
    c->m_estilo.m_cor_fundo = cor(0.07f);
    c->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual;
    c->m_estilo.m_largura = 1;

    for(auto& [nome,dir]: projetos) {
        auto* c = adicionar<container>();
        auto* p = c->nova_tab<painel>(nome);
        c->m_estilo.m_flag_estilo |= flag_estilo::largura_percentual;
        c->m_estilo.m_largura = 1;
        c->m_estilo.m_altura = 115;

        p->m_estilo.m_padding_geral = {20,30};
        p->m_estilo.m_cor_fundo = {0.07,0.07,0.07,1.f};
        p->m_estilo.m_flag_estilo |= flag_estilo::alinhamento_central;
        p->m_estilo.m_orientacao_modular = estilo::orientacao::horizontal;

        p->adicionar<elementos::botao>([this, dir](){
            abrirProjeto(dir);
                }, "", "play.png", 25);
        p->adicionar<elementos::botao>([this, dir](){
            removerProjeto(dir);
            motor::obter().fila_opengl.push([this](){
                configCaixas();
            });
                }, "", "remover.png", 25);
    }
}

/**
 * @brief Remove um projeto existente.
 * Exclui recursivamente o diretório do projeto e atualiza a lista de projetos.
 * O projeto selecionado é redefinido para "nenhum" ou para o primeiro projeto disponível.
 * @param dir O caminho completo do diretório do projeto a ser removido.
 */
void gerenciador_projetos::removerProjeto(const std::string& dir) {
    if(std::filesystem::exists(dir)) {
        std::filesystem::remove_all(dir); // Remove o diretório e todo o seu conteúdo
    }

    buscarProjetos(); // Atualiza a lista de projetos
    // Define o projeto selecionado para 'nenhum' se não houver mais projetos,
    // ou para o primeiro projeto na lista.
    m_projeto_selecionado = projetos.empty() ? "nenhum" : projetos.begin()->first;
}

/**
 * @brief Abre e inicia um projeto no editor.
 * Esta função descarrega recursos existentes, destrói a janela atual (do gerenciador),
 * inicializa uma nova instância do projeto e do sistema do editor, configura o título da janela,
 * e inicia o loop principal do editor.
 * @param caminho O caminho completo para o diretório do projeto a ser aberto.
 */
void gerenciador_projetos::abrirProjeto(const std::string& caminho) {
    motor::obter().fila_opengl.push([caminho](){
        motor::obter().abrirProjeto(caminho);
        motor::obter().m_editor->carregarConfiguracaoPadrao();
    });
}

/**
 * @brief Busca e lista todos os projetos existentes no diretório padrão.
 * Popula o mapa `projetos` com os nomes e caminhos dos projetos encontrados.
 * Se a barra lateral da UI estiver disponível, atualiza dinamicamente os botões
 * para cada projeto encontrado.
 */
void gerenciador_projetos::buscarProjetos() {
    projetos.clear(); // Limpa a lista de projetos anteriores.
    // Itera sobre os subdiretórios no DIR_PADRAO para encontrar projetos.
    if(std::filesystem::exists(DIR_PADRAO))
    for (const auto& entry : std::filesystem::directory_iterator(DIR_PADRAO)) {
        if (entry.is_directory()) {
            auto dir = entry.path().string();
            std::string filename;
            // Procura recursivamente por config
            if(std::filesystem::exists(dir))
                for (const auto& entry : std::filesystem::recursive_directory_iterator(dir)) {
                    if (entry.is_regular_file() && entry.path().extension() == ".bubble") {
                        dir = entry.path().parent_path().string();
                        filename = entry.path().filename().string();
                    }
                }
            else    
                throw  std::runtime_error("Diretório do projeto inexistente.");
        
            std::string full_path = dir + "/" + filename;
            if(std::filesystem::is_regular_file(full_path) == false) continue;
            
            // Executa o parsing
            std::ifstream file(full_path);
            std::stringstream sb;
            sb << file.rdbuf();
            file.close();
        
            rapidjson::Document doc;
            doc.Parse(sb.str().c_str());
            if(!doc.HasMember("nome")) throw std::runtime_error("Analisando projetos do gerenciador de projetos: Projeto sem nome.");
            std::string nome = doc["nome"].GetString();
            projetos[nome] = dir; // Armazena o nome e o diretório do projeto.
            depuracao::emitir(info, "gerenciador_projetos", std::string("Projeto encontrado: ") + nome);
        }
    }
}

void gerenciador_projetos::criarProjeto(const std::string& novo_diretorio, const std::string& nome, const bool padrao) {
    bool hasSpecialChar = false;
    std::string specialChars = R"(/\!@#$%^&*())";
    for (char c : nome) {
        if (specialChars.find(c) != std::string::npos) {
            hasSpecialChar = true;
        }
    }
    if(nome.empty() || hasSpecialChar) {
        depuracao::emitir(erro, "gerenciador_projetos", "nome inválido.");
        return;
    }
    // empty level
    auto fase_string = 
    R"({"nome": "main","entidades":[{"componentes":[{"tipo":"renderizador","modelo":"/cubo"}]}]})";

    std::string diretorioDoProjeto = novo_diretorio + "/" + nome;
    // create project directory
    if(std::filesystem::create_directories(diretorioDoProjeto) == false) {
        depuracao::emitir(erro, "gerenciador_projetos", "criando diretório de novo projeto.");
        return;
    };
    // create levels directory
    if(std::filesystem::create_directories(diretorioDoProjeto + "/fases") == false) {
        depuracao::emitir(erro, "gerenciador_projetos", "criando diretório de fases.");
        return;
    };

    // set up json for project
    rapidjson::Document newDoc;
    newDoc.SetObject();
    rapidjson::Document::AllocatorType& allocator = newDoc.GetAllocator();

    // window configs
    rapidjson::Value _janela(rapidjson::kObjectType);
    _janela.AddMember("largura", 640, allocator);
    _janela.AddMember("altura",  480, allocator);
    _janela.AddMember("titulo",  rapidjson::Value(nome.c_str(), allocator), allocator);
    _janela.AddMember("icone",   rapidjson::Value("scene.png", allocator), allocator);

    newDoc.AddMember("nome", rapidjson::Value(nome.c_str(), allocator), allocator);
    newDoc.AddMember("lancamento", rapidjson::Value("/fases/main", allocator), allocator);
    newDoc.AddMember("janela", _janela, allocator);

    // save file
    std::ofstream ofs(diretorioDoProjeto + "/projeto.bubble");
    if (ofs.is_open()) {
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        newDoc.Accept(writer);
        ofs << buffer.GetString();
        ofs.close();
    }

    std::ofstream fase_file(diretorioDoProjeto + "/fases/main.fase");
    if(fase_file.is_open())
    {
        fase_file << fase_string;
        fase_file.close();
    }

    // save usr

    auto _usr = diretorioDoProjeto + "/.usr";
    std::filesystem::create_directory(_usr); // Cria o diretório se não existir

    auto cam_editor = R"({"fov": 70.0,"zfar": 100.0,"escala": 100.0,"ortho": false,"ceu": [51,51,51,255],"skybox": false,"sens": 20.0,"suav": 0.10000000149011612,"posicao": [-6,4,-2],"rotacao": [-36,16,0],"escala": [1.0,1.0,1.0]})";

    std::ofstream ofsB(_usr + "/cam.json");
    ofsB << cam_editor;
    ofsB.close();

    // Atualiza a lista de projetos disponíveis após a criação do novo projeto.
    buscarProjetos();
}
