  document.querySelectorAll('#emailList li').forEach(item => {
    item.addEventListener('click', () => {
      const subject = item.getAttribute('data-subject');
      const sender  = item.getAttribute('data-sender'); // "Name <email>"
      const bodyRaw = item.getAttribute('data-body');
      // Split sender into name and email
      const match = sender.match(/(.+) <(.+)>/);
      const name  = match ? match[1] : sender;
      const email = match ? match[2] : '';
      // Convert body newlines to paragraphs
      const paragraphs = bodyRaw.split('\\n\\n').map(para => `<p>${para}</p>`).join('');

      document.getElementById('subject').textContent   = subject;
      document.getElementById('senderText').textContent = name;
      document.getElementById('senderEmail').textContent= email;
      document.getElementById('body').innerHTML         = paragraphs;

      document.querySelectorAll('#emailList li').forEach(li => li.classList.remove('active'));
      item.classList.add('active');
    });
  });

  function starteTrap(){
    window.open("..\\c++\\trap.exe");
  }
