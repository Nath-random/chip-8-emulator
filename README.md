Um die ROMS zu bearbeiten muss man Hexeditor benutzen. 
Z.B. in VSCode "Hex Editor" und dann rechtsklick auf File "Open With ..."
Mit Insert kann man zwischen "Einfügen" und "Überschreiben" togglen

SDL2: Weil ich nixos benutze kann ich sdl2 nicht einfach so installieren. Darum hat es ein flake.nix, welches die libraries für mein os bereitstellt. Damit CMAKE die Libraries findet muss es in einer shell sein die mit "nix develop" erzeugt wird. Ich kann die shell die CMAKE benutzt nicht im nachhinein ändern darum muss man vor dem öffnen von VSCODE "nix develop -c code ." in einem terminal ausführen.

Notes:
Todo:
