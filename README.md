# Qt-Agenda

## Introdução
 Esta agenda foi criada com o objetivo de aplicar os conceitos aprendidos sobre **Qt** em um projeto prático e divertido.
 
 A motivação deste projeto foi criar uma agenda simples que separa as tarefas apenas por dia, sem horas específicas ou importancia do evento, e que os dias pudessem ser navegados por meio de um calendário. 
 
## Funcionamento
 
 ![alt text](/img/complete-layout.png "Layout inicial")
 
 **O Layout inicial consite em 3 GroupBoxes.**
 
  + **Calendário:** mostra os dias de cada mês, sendo que os dias com coloração preta se referem a dias de semana sem tarefas, os dias     de coloração vermelha são os finais de semana e os dias em verde correspondem àqueles que possuem tarefas.
  
  + **Ações:** consiste em quatro botões, visto que apenas o _Adicionar Tarefa_ pode ser utilizado sem ter uma tarefa selecionada
     1. _Adicionar Tarefa_: para adicionar una tarefa no dia selecionado;
     2. _Editar Tarefa_: para editar uma tarefa selecionada, inclusive seu dia;
     3. _Deletar Tarefa_: para deletar uma tarefa selecionada;
     4. _Feita / Desfeita_: para marcar uma tarefa feita como desfeita ou vice-versa;
  
  + **Tarefas do dia selecionado:** nesse GroupBox aparecem todas as tarefas existentes para o dia selecionado, separadas por tipo e descrição. O usuário pode interagir com elas selecionando-as e escolhendo uma das ações descritas acima.
  

![alt text](/img/task-layout.png "Layout das Tarefas")

 O **Layout das Tarefas possui três campos**: Data, Tipo e Descrição.
 + Para _adicionar uma tarefa_, obrigatoriamente todos os campos devem ser preenchidos.
 + Para a _edição de uma tarefa_, os campos serão autopreenchidos com o conteúdo atual para que possam ser modificados.
 
## Base de Dados
 A **Base de Dados** foi modelada de uma forma bem simples, uma vez que a quantidade de informações é pequena e não é necessário realizar buscas complexas. Possui uma única tabela com 6 colunas. São elas: _day(INT), month(INT), description(VARCHAR), type(VARCHAR), id(INTEGER PRIMARY KEY AUTOINCREMENT) e done(BOOLEAN DEFAULT (FALSE))_.
