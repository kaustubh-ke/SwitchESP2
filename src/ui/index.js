const socket = io();

const toggleBtn1 = document.getElementById('toggleBtn1');
const toggleBtn2 = document.getElementById('toggleBtn2');

let buttonState1 = false;
let buttonState2 = false;

toggleBtn1.addEventListener('click', () => {
    buttonState1 = !buttonState1;
    updateUI1();
    socket.emit('buttonState1', buttonState1);
});
toggleBtn2.addEventListener('click', () => {
    buttonState2 = !buttonState2;
    updateUI2();
    socket.emit('buttonState2', buttonState2);
});

const updateUI1 = () => {
    buttonState1
        ? toggleBtn1.classList.add('on')
        : toggleBtn1.classList.remove('on');
    toggleBtn1.innerText = buttonState1 ? 'Turn off' : 'Turn on';
};

const updateUI2 = () => {
    buttonState2
        ? toggleBtn2.classList.add('on')
        : toggleBtn2.classList.remove('on');
    toggleBtn2.innerText = buttonState2 ? 'Turn off' : 'Turn on';
};

socket.on('buttonState1', state => {
    console.log('updated state', state);
    buttonState1 = state;
    updateUI1();
});

socket.on('buttonState2', state => {
    console.log('updated state', state);
    buttonState2 = state;
    updateUI2();
});
