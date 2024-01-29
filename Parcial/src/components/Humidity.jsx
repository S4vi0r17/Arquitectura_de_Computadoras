import PropTypes from 'prop-types';
import humidity from '../assets/humidity.svg';

const Humidity = ({ humedad }) => {
	return (
		<div className='h-auto w-full shadow-xl rounded-2xl flex flex-col gap-5 justify-center items-center row-start-3 md:row-start-1 md:col-span-2 lg:col-start-6 lg:col-span-1'>
			<img src={humidity} alt='' className='h-20 w-20' />
			<p className='text-3xl'>{humedad}%</p>
		</div>
	);
};

Humidity.propTypes = {
	humedad: PropTypes.number.isRequired,
};

export default Humidity;
