module Lib (prompt) where

import Text.Read (readMaybe)

prompt :: [String] -> IO ()
prompt todos = do
    putStrLn "Escribe comando:"
    command <- getLine
    case words command of

        ["l"] -> do
            if null todos
                then putStrLn "No hay tareas"
                else mostrarConIndice todos
            prompt todos

        ["q"] -> return ()

        ("+":tarea) -> do
            let nuevos = todos ++ [unwords tarea]
            putStrLn "Tarea agregada"
            prompt nuevos

        ["-", nStr] -> do
            case readMaybe nStr of
                Just n ->
                    if n > 0 && n <= length todos
                        then do
                            let nuevos = eliminar (n-1) todos
                            putStrLn "Tarea eliminada"
                            prompt nuevos
                        else do
                            putStrLn "Índice fuera de rango"
                            prompt todos
                Nothing -> do
                    putStrLn "Número inválido"
                    prompt todos

        _ -> do
            putStrLn "Comando no válido"
            prompt todos

mostrarConIndice :: [String] -> IO ()
mostrarConIndice xs = mapM_ putStrLn (zipWith (\i x -> show i ++ ". " ++ x) [1..] xs)

eliminar :: Int -> [a] -> [a]
eliminar i xs = take i xs ++ drop (i+1) xs