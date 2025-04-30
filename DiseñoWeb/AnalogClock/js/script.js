document.addEventListener('DOMContentLoaded', function() {
  // Configuración inicial
  const clockFace = document.querySelector('.clock-face');
  const numbersContainer = document.querySelector('.numbers-container');
  const minuteMarks = document.querySelector('.minute-marks');
  const themeToggle = document.querySelector('.theme-toggle');
  
  // Generar números del reloj
  for (let i = 1; i <= 12; i++) {
      const number = document.createElement('div');
      number.className = `number number${i}`;
      number.dataset.number = i;
      number.style.setProperty('--rotation', `${i * 30}deg`);
      numbersContainer.appendChild(number);
  }
  
  // Generar marcas de minutos
  for (let i = 0; i < 60; i++) {
      const mark = document.createElement('div');
      mark.className = 'minute-mark';
      mark.style.setProperty('--rotation', `${i * 6}deg`);
      
      // Marcas más grandes cada 5 minutos (horas)
      if (i % 5 === 0) {
          mark.dataset.hourMark = true;
      }
      
      minuteMarks.appendChild(mark);
  }
  
  // Función para actualizar el reloj
  function updateClock() {
      const now = new Date();
      const seconds = now.getSeconds();
      const minutes = now.getMinutes();
      const hours = now.getHours() % 12;
      const date = now.getDate();
      
      const secondHand = document.getElementById('second-hand');
      const minuteHand = document.getElementById('minute-hand');
      const hourHand = document.getElementById('hour-hand');
      const dateDisplay = document.getElementById('date-display');
      
      // Cálculo preciso de los ángulos
      const secondsDegrees = (seconds / 60) * 360 + 90;
      const minutesDegrees = (minutes / 60) * 360 + (seconds / 60) * 6 + 90;
      const hoursDegrees = (hours / 12) * 360 + (minutes / 60) * 30 + 90;
      
      // Aplicar rotaciones
      secondHand.style.transform = `rotate(${secondsDegrees}deg)`;
      minuteHand.style.transform = `rotate(${minutesDegrees}deg)`;
      hourHand.style.transform = `rotate(${hoursDegrees}deg)`;

       // Actualizar fecha
       // Para mostrar solo día y mes numéricos:
       const options = { day: 'numeric', month: 'numeric' };
       dateDisplay.textContent = now.toLocaleDateString('es-ES', options);
      
      // Efecto de "tic-tac" para la manecilla de segundos
      if (seconds === 0) {
          secondHand.style.transition = 'none';
          setTimeout(() => {
              secondHand.style.transition = 'all 0.05s cubic-bezier(0.1, 2.7, 0.58, 1)';
          }, 10);
      }
  }
  
  // Cambiar tema claro/oscuro
  themeToggle.addEventListener('click', function() {
      document.body.dataset.theme = 
          document.body.dataset.theme === 'dark' ? 'light' : 'dark';
      
      // Animación del botón
      this.style.transform = 'rotate(180deg)';
      setTimeout(() => {
          this.style.transform = 'rotate(0deg)';
      }, 300);
  });
  
  // Actualizar el reloj inmediatamente y cada segundo
  updateClock();
  setInterval(updateClock, 1000);
});