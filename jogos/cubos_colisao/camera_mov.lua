local mouseAnterior = vetor2d(0, 0)
local velocidade = 0.02
local distancia = 20
local anguloX = 0
local anguloY = 0

function iniciar()
    eu.transformacao:apontarEntidade(1)
end

function atualizar()
    local mouseAtual = inputs:mouse()
    if inputs.pressionada("MouseE") then
        local mouseDeltaX = mouseAtual.x - mouseAnterior.x
        local mouseDeltaY = mouseAtual.y - mouseAnterior.y

        -- Atualiza os ângulos de rotação
        anguloX = anguloX + mouseDeltaX * velocidade
        anguloY = math.max(-math.pi / 2, math.min(math.pi / 2, anguloY + mouseDeltaY * velocidade))

        -- Calcula nova posição da câmera usando coordenadas esféricas
        eu.transformacao.posicao.x = util.lerp(eu.transformacao.posicao.x, math.cos(anguloY) * math.cos(anguloX) * distancia, 0.1)
        eu.transformacao.posicao.y = util.lerp(eu.transformacao.posicao.y, math.sin(anguloY) * distancia, 0.1)
        eu.transformacao.posicao.z = util.lerp(eu.transformacao.posicao.z, math.cos(anguloY) * math.sin(anguloX) * distancia, 0.1)
    end

    mouseAnterior.x = mouseAtual.x
    mouseAnterior.y = mouseAtual.y
end
