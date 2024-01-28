import { useState, useEffect } from 'react';
import Temperature from './components/Temperature';
import Humidity from './components/Humidity';
import Fan from './components/Fan';

const App = () => {
	const [temperatura, setTemperatura] = useState(null);
	const [humedad, setHumedad] = useState(null);
	const [ventiladorEncendido, setVentiladorEncendido] = useState(false);

	useEffect(() => {
		const fetchData = async () => {
			try {
				const response = await fetch(
					'https://servergbpesp32.onrender.com/api/datos'
				);
				console.log(response);
				const data = await response.json();
				console.log(data);
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
			<div className='mt-20 grid grid-cols-3 grid-rows-2 justify-items-center container mx-auto'>
				<Temperature temperatura={temperatura} />
				<Fan ventiladorEncendido={ventiladorEncendido} />
				<Humidity humedad={humedad} />
			</div>
		</div>
	);
};

export default App;
