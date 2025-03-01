-- Get the parent directory of the script
local script_path = debug.getinfo(1, "S").source:sub(2):match("(.*/)")

-- Add the script's directory to the package path
package.path = package.path .. ";" .. script_path .. "?.lua"

-- Require the gamestate module
local gamestate = require("gamestate")

-- Set the font path dynamically
local parent_path = script_path:match("(.*/).*/")
local font_path = parent_path .. "fontes/minecraft_font.ttf"
local trys_ini
local golf
local pos_ini
local et = 0.0

function iniciar()
    -- Set text color
    gamestate.load()
    eu.texto.cor = cor(0, 0.5, 0.5, 1)
    print (font_path)
    golf = entidade(1)
    
    -- Check if the current phase is "Tutorial"
    if projetoAtual:faseAtual():nome() == "Tutorial" or projetoAtual:faseAtual():nome() ==  "Fase 1" then
        gamestate.trys = 1
        gamestate.write()
    elseif projetoAtual:faseAtual():nome() == "Fase 2" or projetoAtual:faseAtual():nome() ==  "Fase 3" then
        gamestate.trys = 2
        gamestate.write()
    end

    trys_ini = gamestate.trys
    -- Create a copy of the position table
    pos_ini = vetor3(0,0,0)
    
        pos_ini.x = golf.fisica:obtPosicao().x
        pos_ini.y = golf.fisica:obtPosicao().y
        pos_ini.z = golf.fisica:obtPosicao().z
    
    -- Load gamestate
    gamestate.load()
    
    -- Set the font to the dynamically constructed path
        eu.texto.frase = "Tentativas restantes: " .. gamestate.trys
    eu.texto.fonte = font_path
end

function atualizar()
    -- Update the text to show the number of tries
    gamestate.load()
    if gamestate.trys >= 0 then
    eu.texto.frase = "Tentativas restantes: " .. gamestate.trys+1
else
    eu.texto.frase = ""
end
    local velocidadetotal = math.abs(golf.fisica:obtVelocidade().x +
    golf.fisica:obtVelocidade().y +
    golf.fisica:obtVelocidade().z)
    
    if gamestate.trys < 0 and not won and velocidadetotal < 0.1 then
        et = et + tempo.obterDeltaTime()
        gamestate.lost = true
        if et >= 2 then
        gamestate.trys = trys_ini 
        gamestate.lost = false
        golf.fisica:defPosicao(pos_ini)
        et = 0
    end
    gamestate.write()
    end
end