import PropTypes from 'prop-types';
import { useState, useEffect } from 'react';
import {
	AreaChart,
	Area,
	XAxis,
	YAxis,
	CartesianGrid,
	Tooltip,
	ResponsiveContainer, // Para que se adapte al tamaño del contenedor
} from 'recharts';

const Graphic = ({ temperatura, humedad, time }) => {
	const [data, setData] = useState([]);

	useEffect(() => {
		setData((prevData) => {
			const newData = [
				...prevData,
				{ name: time, tem: temperatura, hum: humedad },
			];

			// Mantener solo los últimos 9 puntos
			return newData.length > 9
				? newData.slice(newData.length - 9)
				: newData;
		});
	}, [temperatura, humedad, time]);

	return (
		<ResponsiveContainer width='95%' height={400}>
			<AreaChart
				data={data}
				margin={{
					top: 0,
					right: 30,
					left: 0,
					bottom: 0,
				}}
			>
				<CartesianGrid strokeDasharray='0' />
				<XAxis dataKey='name' />
				<YAxis />
				<Tooltip />

				<Area
					type='monotone'
					dataKey='hum'
					stroke='#98c8ed'
					fill='#98c8ed'
				/>
				<Area
					type='monotone'
					dataKey='tem'
					stroke='#f8644b'
					fill='#f8644b'
				/>
			</AreaChart>
		</ResponsiveContainer>
	);
};

Graphic.propTypes = {
	temperatura: PropTypes.number.isRequired,
	humedad: PropTypes.number.isRequired,
	time: PropTypes.string.isRequired,
};

export default Graphic;
