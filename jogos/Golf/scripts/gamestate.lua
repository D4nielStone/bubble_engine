-- gamestate.lua
GameState = {
    won = false,
    lost = false,
    trys = 0,
    level = 0
}

function GameState.setWin()
    GameState.won = true
end

function GameState.setLose()
    GameState.lost = true
end

function GameState.setTrys(new_trys)
    GameState.trys = new_trys
end

function GameState.write()
    file = io.open(".gamestate", "w")
    file:write(
        string.format(
        "won=%s\nlost=%s\ntrys=%d\nlevel=%d\n",
        tostring(GameState.won),
        tostring(GameState.lost),
        GameState.trys,
        GameState.level)
    )
    file:close()
end

function GameState.load()
    -- Verifica se o arquivo existe
    local file = io.open(".gamestate", "r")
    if file then
        -- Se o arquivo existir, carrega os dados
        for line in io.lines(".gamestate") do
            local key, value = line:match("([^=]+)=([^=]+)")
            if key and value then
                -- Tenta converter para número, caso contrário, verifica se é booleano
                if value == "true" then
                    GameState[key] = true
                elseif value == "false" then
                    GameState[key] = false
                else
                    -- Allow for negative numbers by using tonumber directly
                    GameState[key] = tonumber(value) or value
                end
            end
        end
        file:close()
    else
        -- Se o arquivo não existir, chama a função write para criar o arquivo
        GameState.write()
    end
end



return GameState