module Main where

import Lib (prompt)

main :: IO ()
main = do
    putStrLn "Commands:"
    putStrLn "+ <String> - Add a TODO entry"
    putStrLn "- <Int>    - Delete the numbered entry"
    putStrLn "l          - List todo"
    putStrLn "q          - Quit"
    prompt []