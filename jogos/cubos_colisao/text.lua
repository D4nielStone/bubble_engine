function script_path()
   local str = debug.getinfo(2, "S").source:sub(2)
   return str:match("(.*/)")
end

local textoII

function iniciar()
    textoII = entidade(7).texto
    textoII.refPosAtiva = true
    textoII.fonte = script_path() .. "/minecraft_font.ttf" 
    eu.texto.refPosAtiva = true
    eu.texto.fonte = script_path() .. "/minecraft_font.ttf" --Adiciona fonte
end
function atualizar()
    eu.texto.frase = "Massa: " .. tostring(entidade(4).fisica.massa) -- Mostra massa
    textoII.frase = "Massa: " .. tostring(entidade(5).fisica.massa) -- Mostra massa
    eu.texto.refPos = 
    vetor3(
    entidade(4).transformacao.posicao.x,
    -(entidade(4).transformacao.posicao.y + 2.5),
    entidade(4).transformacao.posicao.z
    )

    textoII.refPos = 
    vetor3(
    entidade(5).transformacao.posicao.x,
    -(entidade(5).transformacao.posicao.y + 5),
    entidade(5).transformacao.posicao.z
    )
end
