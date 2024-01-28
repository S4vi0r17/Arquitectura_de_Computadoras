import { useState, useEffect } from 'react';
import Temperature from './components/Temperature';
import Humidity from './components/Humidity';
import Fan from './components/Fan';
import Graphic from './components/Graphic';

const App = () => {
	// const [temperatura, setTemperatura] = useState(null);
	// const [humedad, setHumedad] = useState(null);
	// const [ventiladorEncendido, setVentiladorEncendido] = useState(false);
	const [temperatura, setTemperatura] = useState(25);
	const [humedad, setHumedad] = useState(45);
	const [ventiladorEncendido, setVentiladorEncendido] = useState(false);
	const [time, setTime] = useState(getFormattedTime());

	useEffect(() => {
		const intervalId = setInterval(() => {
			setTime(getFormattedTime());
		}, 5000);
	}, []);

	function getFormattedTime() {
		const fecha = new Date();
		return `${String(fecha.getHours()).padStart(2, '0')}:${String(
			fecha.getMinutes()
		).padStart(2, '0')}:${String(fecha.getSeconds()).padStart(2, '0')}`;
		//   return `${String(fecha.getHours()).padStart(2, '0')}:${String(fecha.getMinutes()).padStart(2, '0')}`;
	}

	useEffect(() => {
		const fetchData = async () => {
			try {
				// const response = await fetch(
				// 	'https://servergbpesp32.onrender.com/api/datos'
				// );
				const response = await fetch('http://localhost:4000/data');
				// console.log(response);
				const data = await response.json();
				// console.log(data);
				setTemperatura(data.temperatura);
				setHumedad(data.humedad);
				setVentiladorEncendido(data.ventiladorEncendido);
			} catch (error) {
				console.error('Error al obtener datos:', error);
			}
		};

		fetchData();
		const interval = setInterval(fetchData, 5000); // Actualizar cada 5 segundos

		return () => clearInterval(interval);
	}, []);

	return (
		<div className='max-w-[1200px] mx-auto flex flex-col justify-center items-center'>
			<h1 className='text-6xl text-center my-5 font-semibold text-gray-500'>
				Datos del ESP32
			</h1>
			<div className='mt-20 grid gap-5 grid-cols-6 grid-rows-2 justify-items-center container mx-auto'>
				<Temperature temperatura={temperatura} />
				<Fan ventiladorEncendido={ventiladorEncendido} />
				<Humidity humedad={humedad} />
				<div className='col-start-1 col-span-4 row-start-1 row-end-3'>
					<Graphic
						temperatura={temperatura}
						humedad={humedad}
						time={time}
					/>
				</div>
			</div>
		</div>
	);
};

export default App;
