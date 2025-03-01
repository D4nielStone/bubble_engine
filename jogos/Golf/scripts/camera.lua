local ax = 270
local ay = 45
local posicaoAlvo = vetor3(0, 0, 0)
local velocidade = 10 -- Controle da suavidade do movimento
local velocidadeMouse = 10 -- Controle da suavidade do movimento do mouse
local s = 50
local mouseXAnterior = 0
local mouseYAnterior = 0
local deltaMouseX = 0
local deltaMouseY = 0
local bola
local gatilho = false
local escalaAntiga

-- Obtém o diretório do próprio script
local script_path = debug.getinfo(1, "S").source:sub(2):match("(.*/)")
package.path = package.path .. ";" .. script_path .. "?.lua"
local modulo = require("arrastando")

function iniciar()
    -- Configurações da câmera
    eu.camera.flag_ortho= false
    eu.camera.fov = 10
    eu.camera.escala = s
    eu.camera:ativarFB()
    eu.transformacao.posicao = vetor3(s * 3, s * 3, s * 3) -- Posição inicial da câmera
    bola = entidade(1)
    eu.transformacao:apontarEntidade(bola.id)

    -- Inicializa os valores do mouse
    local mouse = inputs:mouse()
    mouseXAnterior = mouse.x
    mouseYAnterior = mouse.y
end

function atualizar()
    local deltaTime = tempo.obterDeltaTime() * velocidade

    -- Calcula a distância entre a câmera e a bola
    local distancia = util.distanciaV3(bola.transformacao.posicao, eu.transformacao.posicao)

    -- Define a velocidade proporcional à distância, com limite mínimo
    local velocidadeAproximacao = math.max(0.01, distancia * deltaTime * 0.05)

    local vb = bola.fisica:obtVelocidade().x + bola.fisica:obtVelocidade().y + bola.fisica:obtVelocidade().z

    if not gatilho or math.abs(vb) > 0.7 then
        -- Suaviza a posição da câmera usando lerp e deltaTime
        eu.transformacao.posicao.x = util.lerp(
            eu.transformacao.posicao.x,
            posicaoAlvo.x + bola.transformacao.posicao.x,
            0.01 * deltaTime
        )
        eu.transformacao.posicao.y = util.lerp(
            eu.transformacao.posicao.y,
            posicaoAlvo.y + bola.transformacao.posicao.y,
            0.01 * deltaTime
        )
        eu.transformacao.posicao.z = util.lerp(
            eu.transformacao.posicao.z,
            posicaoAlvo.z + bola.transformacao.posicao.z,
            0.01 * deltaTime
        )
    else
        -- Suaviza a posição da câmera usando lerp e deltaTime
        eu.transformacao.posicao.x = util.lerp(
            eu.transformacao.posicao.x,
            posicaoAlvo.x + bola.transformacao.posicao.x,
            velocidadeAproximacao * deltaTime
        )
        eu.transformacao.posicao.y = util.lerp(
            eu.transformacao.posicao.y,
            posicaoAlvo.y + bola.transformacao.posicao.y,
            velocidadeAproximacao * deltaTime
        )
        eu.transformacao.posicao.z = util.lerp(
            eu.transformacao.posicao.z,
            posicaoAlvo.z + bola.transformacao.posicao.z,
            velocidadeAproximacao * deltaTime
        )
    end

    -- Obtém a posição atual do mouse
    local mouse = inputs:mouse()

    -- Calcula o delta do movimento do mouse
    deltaMouseX = (mouse.x - mouseXAnterior) * velocidadeMouse
    deltaMouseY = (mouse.y - mouseYAnterior) * velocidadeMouse

    -- Atualiza os valores anteriores do mouse
    mouseXAnterior = mouse.x
    mouseYAnterior = mouse.y

    local arrastando = modulo.arrastando(inputs:tamanhoTela().y / 12)

    -- Calcula o ângulo baseado no delta do movimento do mouse
    if inputs.pressionada("MouseE") and not arrastando and math.abs(vb) < 0.7 then
        gatilho = true
        ax = ax + (deltaMouseX / 10) * deltaTime
        ay = ay + (deltaMouseY / 10) * deltaTime -- Inverte o sinal para ajustar elevação
        ay = math.max(15, math.min(75, ay)) -- Clamping para manter dentro dos limites
    end

    -- Converte os ângulos em radianos para cálculos
    local radAx = math.rad(ax)
    local radAy = math.rad(ay)

    -- Calcula a nova posição alvo com base em ax e ay
    posicaoAlvo = vetor3(
        math.cos(radAx) * math.cos(radAy) * eu.camera.escala * 2,
        math.sin(radAy) * eu.camera.escala,
        math.sin(radAx) * math.cos(radAy) * eu.camera.escala * 2
    )
end
