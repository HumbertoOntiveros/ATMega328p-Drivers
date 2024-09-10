# Configurar y abrir el puerto COM
$port = new-Object System.IO.Ports.SerialPort COM4,9600,None,8,one
$port.Open()

# Mensaje de inicio
Write-Host @"
___  ___            _ _               _____           _       _ 
|  \/  |           (_) |             /  ___|         (_)     | |
| .  . | ___  _ __  _| |_ ___  _ __  \ `--.   ___ _ __ _  __ _| |
| |\/| |/ _ \| '_ \| | __/ _ \| '__|  \--. \/ _ \ '__| |/ _' | |
| |  | | (_) | | | | | || (_) | |    /\__/ /  __/ |  | | (_| | |
\_|  |_/\___/|_| |_|_|\__\___/|_|    \____/ \___|_|  |_|\__,_|_|

-----------------------------------------------------------------
Connected to COM4. Listening... 
Type your message and press Enter to send. 
Press Esc to exit.
-----------------------------------------------------------------
"@

#Write-Host "Connected to COM4. Listening... Type your message and press Enter to send. Press Esc to exit."

# Buffer para almacenar el texto antes de enviarlo
$messageBuffer = ""
$data = ""

# Leer datos y enviar al puerto serial
while ($port.IsOpen) {

    if ($port.BytesToRead -gt 0) {
        try {
            $availableData = $port.ReadExisting()  # Lee todos los datos disponibles en este momento
            $data += $availableData

            # Verifica si se ha completado una línea
            if ($data -match "`r`n" -or $data -match "`n") {
                # Imprime la línea completa y limpia el buffer
                Write-Host -NoNewline "Rx: $data"
                $data = ""  # Limpiar el buffer
            }
        }
        catch {
            Write-Host "Error reading from COM port: $_"
        }
    }

    # Verificar si hay alguna tecla presionada
    if ([console]::KeyAvailable) {
        $key = [console]::ReadKey($true)  # Lee la tecla sin mostrarla en la consola

        # Salir si se presiona la tecla Esc
        if ($key.Key -eq [ConsoleKey]::Escape) {
            Write-Host "`nEscape key pressed. Exiting..."
            break
        }

        # Manejar la entrada del usuario
        if ($key.Key -eq [ConsoleKey]::Enter) {
            if ($messageBuffer.Length -gt 0) {
                $port.WriteLine($messageBuffer)  # Enviar el buffer al puerto serial
                Write-Host "Tx: $messageBuffer"  # Mostrar y limpiar el buffer en la consola
                $messageBuffer = ""  # Limpiar el buffer
            }
        }
        else {
            $messageBuffer += $key.KeyChar  # Agregar el carácter al buffer
            #Write-Host -NoNewline $key.KeyChar  # Mostrar el carácter en la consola
        }
    }

    #Start-Sleep -Milliseconds 1  # Evita sobrecargar la CPU
}

# Cerrar el puerto COM
$port.Close()
Write-Host "Port closed."
