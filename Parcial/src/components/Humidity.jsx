import PropTypes from 'prop-types';
import humidity from '../assets/humidity.svg';

const Humidity = ({ humedad }) => {
	return (
		<div className='h-52 w-52 shadow-xl rounded-2xl flex flex-col gap-5 justify-center items-center col-start-3'>
			<img src={humidity} alt='' className='h-20 w-20' />
			<p className='text-4xl'>{humedad}%</p>
		</div>
	);
};

Humidity.propTypes = {
	humedad: PropTypes.number.isRequired,
};

export default Humidity;
