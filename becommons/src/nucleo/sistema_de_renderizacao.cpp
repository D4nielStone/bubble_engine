#include <glad/glad.h>
#include "nucleo/sistema_de_renderizacao.hpp"
#include "componentes/renderizador.hpp"
#include "componentes/transformacao.hpp"
#include "componentes/luz_direcional.hpp"
#include "componentes/camera.hpp"
#include "componentes/terreno.hpp"
#include "nucleo/fase.hpp"
#include "arquivadores/shader.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "os/janela.hpp"
#include "depuracao/debug.hpp"

namespace bubble
{
    void sistemaRenderizacao::atualizar()
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        auto camera = _Mfase->obterCamera();
        if (!camera) 
        {
            depuracao::emitir(debug, "fase", "Camera não configurada");
            return;
        }

        camera->desenharFB();

        luz_direcional ld;

        reg->cada<luz_direcional>([&](const uint32_t ent) {
            if(reg->tem<luz_direcional>(ent)) ld = *reg->obter<luz_direcional>(ent);
        });
        reg->cada<transformacao>([&](const uint32_t ent) {
            auto transform = reg->obter<transformacao>(ent);
            transform->calcular();
            });
        reg->cada<renderizador, transformacao>([&](const uint32_t ent_ren) {
            auto render = reg->obter<renderizador>(ent_ren);
            auto transform = reg->obter<transformacao>(ent_ren);
            
            auto s = render->modelo->shader();

            s.use();
            s.setMat4("view", glm::value_ptr(camera->obtViewMatrix()));
            s.setVec3("dirLight.direction", ld.direcao);
            s.setVec3("dirLight.ambient", ld.ambiente);
            s.setVec3("dirLight.specular", ld.especular);
            s.setVec3("dirLight.diffuse", ld.difusa);
            s.setVec3("viewPos", camera->posicao.x,camera->posicao.y,camera->posicao.z);
            s.setMat4("projection", glm::value_ptr(camera->obtProjectionMatrix()));
            s.setVec2("resolution", instanciaJanela->tamanho.x, instanciaJanela->tamanho.y);
            s.setMat4("modelo", transform->obter());
            render->modelo->desenhar(s);
            });
        reg->cada<terreno, transformacao>([&](const uint32_t ent) {
            auto terr = reg->obter<terreno>(ent);
            auto transform = reg->obter<transformacao>(ent);
            
            auto s = terr->shader();

            s.use();
            s.setMat4("view", glm::value_ptr(camera->obtViewMatrix()));
            s.setVec3("dirLight.direction", ld.direcao);
            s.setVec3("dirLight.ambient", ld.ambiente);
            s.setVec3("dirLight.specular", ld.especular);
            s.setVec3("dirLight.diffuse", ld.difusa);
            s.setVec3("viewPos", camera->posicao.x,camera->posicao.y,camera->posicao.z);
            s.setMat4("projection", glm::value_ptr(camera->obtProjectionMatrix()));
            s.setVec2("resolution", instanciaJanela->tamanho.x, instanciaJanela->tamanho.y);
            s.setMat4("modelo", transform->obter());
            terr->desenhar(s);
        });

    if(camera->flag_fb){
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, instanciaJanela->tamanho.x, instanciaJanela->tamanho.y);
    }
    }

    void sistemaRenderizacao::inicializar(bubble::fase* fase_ptr)
    {
        this->_Mfase = fase_ptr;
        this->reg = _Mfase->obterRegistro();

        glCullFace(GL_BACK);
    }
}