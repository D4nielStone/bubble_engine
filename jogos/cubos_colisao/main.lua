local gatilho = false
local cuboI
local cuboII
local cont = 0
local colidindoA = false
local colidindoB = false

function iniciar()
    cuboII = entidade(8)

    
    local tex = textura(0, "A")
    
    entidade(11).camera:ativarFB()
    
    tex.id = entidade(11).camera.framebuffer 
    
    entidade(11).camera:viewport(vetor2d(500, 500))

    entidade(4).renderizador.modelo:obtMalha(0).material.albedo = tex  
    cuboII.fisica:defFriccao(0)
    cuboII.fisica:defRestituicao(1)
    cuboII.fisica:defFatorAngular(vetor3(0, 0, 0))
    cuboII.fisica:defFatorLinear(vetor3(1, 0, 0))

    cuboI = entidade(4)
    cuboI.fisica:defFriccao(0)
    cuboI.fisica:defRestituicao(1)
    cuboI.fisica:defFatorAngular(vetor3(0, 0, 0))
    cuboI.fisica:defFatorLinear(vetor3(1, 0, 0))
    eu.fisica:defFriccao(0)
    eu.fisica:defRestituicao(1)
    eu.fisica:defFatorAngular(vetor3(0, 0, 0))
    eu.fisica:defFatorLinear(vetor3(1, 0, 0))

    eu.fisica:defRaioCcd(0.01)
    cuboI.fisica:defRaioCcd(0.01)
    eu.fisica:defVelocidade(vetor3(0,0,0))
    cuboI.fisica:defVelocidade(vetor3(0,0,0))
end

function atualizar()
    if not gatilho and inputs.pressionada("Scrl") then
        eu.fisica:defVelocidade(vetor3(-20, 0, 0))
        gatilho = true
    end
    
    local numcoliA = fisica.numColisoes(eu.fisica:corpoRigido(), cuboI.fisica:corpoRigido()) > 0
    local numcoliB = fisica.numColisoes(cuboII.fisica:corpoRigido(), cuboI.fisica:corpoRigido()) > 0

    -- Detecta início da colisão para somar apenas uma vez
    if numcoliA and not colidindoA then
        cont = cont + 1
        colidindoA = true
    elseif not numcoliA then
        colidindoA = false
    end

    if numcoliB and not colidindoB then
        cont = cont + 1
        colidindoB = true
    elseif not numcoliB then
        colidindoB = false
    end

    entidade(9).texto.frase = "#colisoes: " .. (tostring(cont))
end
